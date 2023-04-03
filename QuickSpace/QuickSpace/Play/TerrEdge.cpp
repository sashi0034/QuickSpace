#include "stdafx.h"
#include "TerrEdge.h"

#include "QuickSpace/Util/Range.h"

namespace QuickSpace::Play
{
	TerrEdge::TerrEdge() = default;

	TerrEdge::TerrEdge(const TerrVertexRef& startPos, const TerrVertexRef& endPos) :
		m_startPos(startPos),
		m_endPos(endPos),
		m_direction(Angle::ConvertFrom(endPos->xy() - startPos->xy()).Value())
	{}

	Vec2 TerrEdge::Midpoint(float rate) const
	{
		return (*m_startPos) * rate + (*m_endPos) * (1 - rate);
	}

	TerrVertexRef TerrEdge::GetStart() const
	{
		return m_startPos;
	}

	TerrVertexRef TerrEdge::GetEnd() const
	{
		return m_endPos;
	}

	bool TerrEdge::HasVertex(const TerrVertexRef& vertex) const
	{
		return m_startPos.get() == vertex.get() || m_endPos.get() == vertex.get();
	}

	Optional<EAngle> TerrEdge::GetDirectionOf(const TerrVertexRef& oneSideVertex) const
	{
		if (m_startPos.get() == oneSideVertex.get())
		{
			return m_direction;
		}
		if (m_endPos.get() == oneSideVertex.get())
		{
			return Angle(m_direction).Reverse();
		}
		return none;
	}

	void TerrEdge::MoveOnEdge(float* movingRate, EAngle direction, float speed)
	{
		if (direction == m_direction)
			*movingRate -= speed;
		else if (direction == Angle(m_direction).Reverse())
			*movingRate += speed;
		*movingRate = Util::RangeF(0, 1).Clamp(*movingRate);
	}

	bool TerrEdge::IsHorizontal() const
	{
		return m_direction == EAngle::Left || m_direction == EAngle::Right;
	}

	bool TerrEdge::IsNeighborWith(const TerrEdge& other)
	{
		const bool isThisHorizontal = IsHorizontal();

		// 辺の向きが同じ
		if (isThisHorizontal == other.IsHorizontal())
		{
			return
				m_startPos.get() == other.m_startPos.get() || m_startPos.get() == other.m_endPos.get() ||
				m_endPos.get() == other.m_startPos.get() || m_endPos.get() == other.m_endPos.get();
		}

		// 辺の向きが違う
		if (isThisHorizontal)
			// 自身が水平
			return
				Util::RangeInt::FromSort(m_startPos->x, other.m_endPos->x).IsBetween(other.m_startPos->x) &&
				m_startPos->y == other.m_startPos->y && m_startPos->y == other.m_endPos->y;
		else
			// 自身が垂直
			return
				Util::RangeInt::FromSort(m_startPos->y, other.m_endPos->y).IsBetween(other.m_startPos->y) &&
				m_startPos->y == other.m_startPos->y && m_startPos->y == other.m_endPos->y;
	}

	const Array<TerrEdgeNeighbor>& TerrEdge::Neighbors() const
	{
		return m_neighbors;
	}

	void TerrEdge::ConnectEdges(const TerrEdgeRef& neighbor1, const TerrEdgeRef& neighbor2)
	{
		assert(neighbor1->IsHorizontal() != neighbor2->IsHorizontal());

		neighbor1->addNeighbor(neighbor2);
		neighbor2->addNeighbor(neighbor1);
	}

	void TerrEdge::addNeighbor(const TerrEdgeRef& other)
	{
		TerrVertexRef overlappedVertex{};
		if (IsHorizontal())
		{
			if (m_startPos->y == other->m_startPos->y) overlappedVertex =  other->m_startPos;
			if (m_startPos->y == other->m_endPos->y) overlappedVertex =  other->m_endPos;
		}
		else
		{
			if (m_startPos->x == other->m_startPos->x) overlappedVertex =  other->m_startPos;
			if (m_startPos->x == other->m_endPos->x) overlappedVertex =  other->m_endPos;
		}

		if (overlappedVertex.get() == nullptr)
		{
			assert(false);
			return;
		}

		m_neighbors.push_back(TerrEdgeNeighbor{other, overlappedVertex });
	}
}
