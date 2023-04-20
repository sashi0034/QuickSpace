#include "stdafx.h"
#include "SepEdgeSet.h"

#include "SepFace.h"

namespace QuickSpace::Play
{
	void SepEdgeSet::TestCase()
	{
		testClockwise(
			Array<Point>{
			  {862, 160}, {862, 627}, {912, 627}, {912, 160}},
			 false);
		testClockwise(
			Array<Point>{
			  {862, 85}, {862, 627}, {912, 627}, {912, 156},{1198, 156},
			  {1198, 85}},
			 false);
		testClockwise(
			Array<Point>{
	          {862, 160}, {862, 627}, {912, 627}, {912, 156},{1198, 156},
	          {1198, 60},{1012, 60}, {1012, 85}, {895, 85}, {895, 160}},
	         false);
	}

	SepEdgeSet SepEdgeSet::CreateFromVertexes(const Array<Point>& points)
	{
		SepEdgeSet edgeSet = {};
		for (int i1=0; i1<points.size(); ++i1)
		{
			const int i2 = (i1 + 1) % points.size();
			edgeSet.Edges().push_back(SepEdge(points[i1], points[i2]));
		}
		return edgeSet;
	}

	void SepEdgeSet::testClockwise(const Array<Point>& points, bool isClockwise)
	{
		SepEdgeSet edgeSet = CreateFromVertexes(points);
		assert(edgeSet.isClockwiseAsCircuit() == isClockwise);
	}

	SepEdgeSet::SepEdgeSet(const Array<SepEdge>& edges) :
		m_edges(edges)
	{}

	void SepEdgeSet::TryDivideEdge(const TerrVertex& vertex)
	{
		for (auto&& edge : m_edges)
		{
			// 既存の辺の端点のどこかと重なっているなら分割中止
			if (edge.GetStart() == vertex || edge.GetEnd() == vertex) return;
		}

		int overlappedIndex = -1;
		for (int i = 0; i< m_edges.size(); ++i)
		{
			if (m_edges[i].IsOverlappedVertex(vertex) == false) continue;;
			overlappedIndex = i;
			break;
		}
		if (overlappedIndex == -1) return;

		// 重なってる辺があったので分割適応
		auto overlappedEdge = m_edges[overlappedIndex];
		m_edges.remove_at(overlappedIndex);
		m_edges.push_back(SepEdge(overlappedEdge.GetStart(), vertex));
		m_edges.push_back(SepEdge(vertex, overlappedEdge.GetEnd()));
	}


	SepEdgeSetTwoRoot SepEdgeSet::CalcRouteAsPureCircuit(const Point& startPoint, const Point& endPoint)
	{
		if (startPoint == endPoint) return SepEdgeSetTwoRoot{{}, *this};

		auto checkedFlags = Array<bool>(m_edges.size());

		// スタート地点の辺、2箇所を探す
		int startEdgeId1 = -1;
		int startEdgeId2 = -1;
		for (int i=0; i<m_edges.size(); ++i)
		{
			auto&& edge = m_edges[i];
			if (edge.GetStart() != startPoint &&
				edge.GetEnd() != startPoint) continue;

			checkedFlags[i] = true;
			if (startEdgeId1 == -1) startEdgeId1 = i;
			else if (startEdgeId2 == -1) startEdgeId2 = i;
			else {assert(false);}
		}

		SepEdgeSet route1{};
		SepEdgeSet route2{};

		route1.m_edges.push_back(m_edges[startEdgeId1]);
		route2.m_edges.push_back(m_edges[startEdgeId2]);

		followAndConnectEdges(&route1, &checkedFlags, endPoint, true);
		followAndConnectEdges(&route2, &checkedFlags, endPoint, true);

		int routeDistance1 = route1.sumEdgeDistance();
		int routeDistance2 = route2.sumEdgeDistance();

		return routeDistance1 < routeDistance2
			? SepEdgeSetTwoRoot{route1, route2}
			: SepEdgeSetTwoRoot{route2, route1};
	}

	Array<SepEdge>& SepEdgeSet::Edges()
	{
		return m_edges;
	}

	SepEdgeSet SepEdgeSet::CreateClockwiseCircuit()
	{
		if (m_edges.size() == 0) return *this;

		SepEdgeSet sortedSet{};
		auto checkedFlags = Array<bool>(m_edges.size());

		sortedSet.m_edges.push_back(m_edges[0]);
		checkedFlags[0] = true;

		// 先頭の辺の始点を終端としてつないでいく
		followAndConnectEdges(&sortedSet, &checkedFlags, m_edges[0].GetStart(), false);
		// すぐに始点を含む辺とつながったらサイズ2で終了してしまうので、終点を終端とする
		if (m_edges.size() != sortedSet.m_edges.size())
			followAndConnectEdges(&sortedSet, &checkedFlags, m_edges[0].GetEnd(), false);

		// 各辺の始点と終点がつながるように
		for (int i1=0; i1 <= sortedSet.m_edges.size() - 1; ++i1)
		{
			const int i2 = (i1 + 1) % sortedSet.m_edges.size();
			if (sortedSet.m_edges[i1].GetStart() == sortedSet.m_edges[i2].GetStart() ||
				sortedSet.m_edges[i1].GetStart() == sortedSet.m_edges[i2].GetEnd())
				sortedSet.m_edges[i1].SwapStartAndEnd();
		}

		// 反時計回りだったら時計回りにする
		if (sortedSet.isClockwiseAsCircuit() == false)
		{
			sortedSet.m_edges.reverse();
			for (auto&& edge : sortedSet.m_edges) edge.SwapStartAndEnd();
		}

		return sortedSet;
	}

	Polygon SepEdgeSet::ConstructPolygon() const
	{
		const auto polygonPoints =
			m_edges.map([](SepEdge edge){ return Vec2(edge.GetStart().xy()); });

		return Polygon(polygonPoints);
	}

	int SepEdgeSet::sumEdgeDistance()
	{
		int sum = 0;
		for (auto&& edge : m_edges)
		{
			sum += edge.GetLength();
		}
		return sum;
	}

	bool SepEdgeSet::isClockwiseAsCircuit()
	{
		// 左端の垂直方向の辺が上向きかどうかで時計回りを判定

		int mostLeftIndex = -1;
		for (int i=0; i<m_edges.size(); ++i)
		{
			if (m_edges[i].IsHorizontal()) continue;

			const bool isUpdate = mostLeftIndex == -1 ||
				(m_edges[i].GetStart().x < m_edges[mostLeftIndex].GetStart().x);
			if (isUpdate == false) continue;

			mostLeftIndex = i;
		}
		assert(mostLeftIndex != -1);
		if (mostLeftIndex == -1) return false;
		return m_edges[mostLeftIndex].GetDirection().Value() == EAngle::Up;
	}

	void SepEdgeSet::followAndConnectEdges(
		SepEdgeSet* routeRef, Array<bool>* checkedFlags, const Point& endPoint, bool canFinishAtFrontEdge)
	{
		while (true)
		{
			// 終点につながるまで操作
			if (canFinishAtFrontEdge && routeRef->m_edges.back().IsTipVertex(endPoint)) break;
			canFinishAtFrontEdge = true;

			// 計算経路の配列の末尾要素につながるものを足していく
			for (int i = 0;; ++i)
			{
				if (i==m_edges.size())
				{
					assert(false);
					return;
				}

				if ((*checkedFlags)[i]) continue;
				auto&& edge = m_edges[i];
				if (routeRef->m_edges.back().IsShareVertexWith(edge) == false) continue;

				// 末尾要素の始点か終点につながったら追加

				(*checkedFlags)[i] = true;
				routeRef->m_edges.push_back(edge);
				break;
			}
		}
	}
}
