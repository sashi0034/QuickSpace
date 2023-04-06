#include "stdafx.h"
#include "SepEdge.h"

namespace QuickSpace:: Play
{
	SepEdge::SepEdge(const TerrVertexRef& start, const TerrVertexRef& end) :
		m_start(start),
		m_end(end)
	{}

	SepEdge::SepEdge(const TerrEdge& edge) :
		m_start(edge.GetStart()),
		m_end(edge.GetEnd())
	{}

	SepEdge::SepEdge(const TerrEdgeRef& edge) :
		SepEdge(*edge)
	{}

	bool SepEdge::IsIntersectWith(const SepEdge& other) const
	{
		if (isHorizontal() == other.isHorizontal()) return false;
		if (isHorizontal())
			return
				Util::RangeInt::FromSort(m_start->x, m_end->x).IsBetween(other.m_start->x) &&
				Util::RangeInt::FromSort(other.m_start->y, other.m_end->y).IsBetween(m_start->y);
		else
			return
				Util::RangeInt::FromSort(m_start->y, m_end->y).IsBetween(other.m_start->y) &&
				Util::RangeInt::FromSort(other.m_start->x, other.m_end->x).IsBetween(m_start->x);
	}

	bool SepEdge::IsOverlappedVertex(const TerrVertexRef& vertex) const
	{
		return TerrEdge::IsOverlappedVertexBetween(m_start, m_end, *vertex);
	}

	bool SepEdge::IsOverlappedVertex(const Point& vertex) const
	{
		return TerrEdge::IsOverlappedVertexBetween(m_start, m_end, vertex);
	}

	bool SepEdge::IsShareVertexWith(const SepEdge& other) const
	{
		return	*m_start == *other.m_start ||
				*m_start == *other.m_end ||
				*m_end == *other.m_start ||
				*m_end == *other.m_end;
	}

	int SepEdge::GetLength() const
	{
		return (*m_start - *m_end).manhattanLength();
	}

	Point SepEdge::CalcIntersected(const SepEdge& other) const
	{
		// assert(IsIntersectWith(other));
		return isHorizontal()
			? Point(other.m_start->x, m_start->y)
			: Point(m_start->x, other.m_start->y);
	}

	TerrVertexRef& SepEdge::GetStart()
	{
		return m_start;
	}

	TerrVertexRef& SepEdge::GetEnd()
	{
		return m_end;
	}

	Angle SepEdge::GetDirection() const
	{
		return Angle(TerrEdge::CalcDirectionBetween(m_start, m_end));
	}

	bool SepEdge::isHorizontal() const
	{
		return m_start->y == m_end->y;
	}
}
