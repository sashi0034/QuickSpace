#include "stdafx.h"
#include "SepEdgeSet.h"

#include "SepFace.h"

namespace QuickSpace::Play
{
	SepEdgeSet::SepEdgeSet(const Array<SepEdge>& edges) :
		m_edges(edges)
	{}

	void SepEdgeSet::TryDivideEdge(const TerrVertexRef& vertex)
	{
		for (auto&& edge : m_edges)
		{
			// 既存の辺の端点のどこかと重なっているなら分割中止
			if (*edge.GetStart() == *vertex || *edge.GetEnd() == *vertex) return;
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


	SepEdgeSetTwoRoot SepEdgeSet::CalcRootAsPureCircuit(const Point& startPoint, const Point& endPoint)
	{
		if (startPoint == endPoint) return SepEdgeSetTwoRoot{{}, *this};

		auto checkedFlags = Array<bool>(m_edges.size());

		// スタート地点の辺、2箇所を探す
		int startEdgeId1 = -1;
		int startEdgeId2 = -1;
		for (int i=0; i<m_edges.size(); ++i)
		{
			auto&& edge = m_edges[i];
			if (*edge.GetStart() != startPoint &&
				*edge.GetEnd() != startPoint) continue;

			checkedFlags[i] = true;
			if (startEdgeId1 == -1) startEdgeId1 = i;
			else if (startEdgeId2 == -1) startEdgeId2 = i;
			else {assert(false);}
		}

		SepEdgeSet root1{};
		SepEdgeSet root2{};

		root1.m_edges.push_back(m_edges[startEdgeId1]);
		root2.m_edges.push_back(m_edges[startEdgeId2]);

		followAndConnectEdges(&root1, &checkedFlags, endPoint);
		followAndConnectEdges(&root2, &checkedFlags, endPoint);

		int rootDistance1 = root1.sumEdgeDistance();
		int rootDistance2 = root2.sumEdgeDistance();

		return rootDistance1 < rootDistance2
			? SepEdgeSetTwoRoot{root1, root2}
			: SepEdgeSetTwoRoot{root2, root1};
	}

	Array<SepEdge>& SepEdgeSet::Edges()
	{
		return m_edges;
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

	void SepEdgeSet::followAndConnectEdges(SepEdgeSet* rootRef, Array<bool>* checkedFlags, const Point& endPoint)
	{
		while (true)
		{
			// 終点につながるまで操作
			if (*rootRef->m_edges.back().GetEnd() == endPoint || *rootRef->m_edges.back().GetStart() == endPoint) break;

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
				if (*rootRef->m_edges.back().GetEnd() != *edge.GetStart() &&
					*rootRef->m_edges.back().GetStart() != *edge.GetEnd()) continue;

				// 末尾要素の始点か終点につながったら追加

				(*checkedFlags)[i] = true;
				rootRef->m_edges.push_back(edge);
				break;
			}
		}
	}
}
