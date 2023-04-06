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
		void TryDivideEdge(const TerrVertexRef& vertex);

		// 純粋な閉路から経路探索
		SepEdgeSetTwoRoot CalcRootAsPureCircuit(const Point& startPoint, const Point& endPoint);

		Array<SepEdge>& Edges();
	private:
		Array<SepEdge> m_edges{};

		void followAndConnectEdges(SepEdgeSet* rootRef, Array<bool>* checkedFlags, const Point& endPoint);
		int sumEdgeDistance();
	};

	struct SepEdgeSetTwoRoot
	{
		SepEdgeSet ShortRoot;
		SepEdgeSet LongRoot;
	};

}
