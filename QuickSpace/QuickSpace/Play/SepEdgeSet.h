#pragma once
#include "SepEdge.h"

namespace QuickSpace::Play
{
	class SepFace;
	class SepEdgeSetTwoRoot;

	class SepEdgeSet
	{
	public:
		SepEdgeSet() = default;
		explicit SepEdgeSet(const Array<SepEdge>& edges);
		void TryDivideEdge(const TerrVertex& vertex);

		// 純粋な閉路から経路探索
		SepEdgeSetTwoRoot CalcRouteAsPureCircuit(const Point& startPoint, const Point& endPoint);

		Array<SepEdge>& Edges();
		SepEdgeSet CreateClockwiseCircuit();
		Polygon ConstructPolygon() const;

		static void TestCase();
	private:
		Array<SepEdge> m_edges{};

		void followAndConnectEdges(SepEdgeSet* routeRef, Array<bool>* checkedFlags, const Point& endPoint, bool canFinishAtFrontEdge);
		int sumEdgeDistance();
		bool isClockwiseAsCircuit();

		static void testClockwise(Array<Point> points, bool isClockwise);
	};

	struct SepEdgeSetTwoRoot
	{
		SepEdgeSet ShortRoot;
		SepEdgeSet LongRoot;
	};

}
