#pragma once
#include "TerrEdge.h"

namespace QuickSpace::Play
{
	class SepEdge
	{
	public:
		SepEdge(const TerrVertexRef& start, const TerrVertexRef& end);
		explicit SepEdge(const TerrEdge& edge);
		explicit SepEdge(const TerrEdgeRef& edge);
		bool IsIntersectWith(const SepEdge& other) const;
		bool IsOverlappedVertex(const TerrVertexRef& vertex) const;
		bool IsOverlappedVertex(const Point& vertex) const;
		bool IsShareVertexWith(const SepEdge& other) const;
		int GetLength() const;
		Point CalcIntersected(const SepEdge& other) const;
		TerrVertexRef& GetStart();
		TerrVertexRef& GetEnd();
		Angle GetDirection() const;
	private:
		bool isHorizontal() const;

		TerrVertexRef m_start{};
		TerrVertexRef m_end{};
	};
}
