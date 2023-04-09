#pragma once
#include "TerrEdge.h"

namespace QuickSpace::Play
{
	class SepEdge
	{
	public:
		SepEdge(const TerrVertex& start, const TerrVertex& end);
		explicit SepEdge(const TerrEdge& edge);
		explicit SepEdge(const TerrEdgeRef& edge);
		bool IsIntersectWith(const SepEdge& other) const;
		bool IsOverlappedVertex(const TerrVertex& vertex) const;
		bool IsTipVertex(const Point& other) const;
		bool IsShareVertexWith(const SepEdge& other) const;
		int GetLength() const;
		Point CalcIntersected(const SepEdge& other) const;
		TerrVertex& GetStart();
		TerrVertex& GetEnd();
		void ChangeEnd(const TerrVertex& point);
		Point GetVec() const;
		Angle GetDirection() const;
		void SwapStartAndEnd();
		bool IsHorizontal() const;
	private:

		TerrVertex m_start{};
		TerrVertex m_end{};
	};
}
