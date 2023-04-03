#include "stdafx.h"
#include "TerritoryEdge.h"

#include "QuickSpace/Util/Range.h"

namespace QuickSpace::Play
{
	TerritoryEdge::TerritoryEdge() = default;

	TerritoryEdge::TerritoryEdge(const TerritoryVertex& startPos, const TerritoryVertex& endPos) :
		m_startPos(startPos),
		m_endPos(endPos),
		m_direction(Angle::ConvertFrom(endPos->xy() - startPos->xy()).Value())
	{}

	Vec2 TerritoryEdge::Midpoint(float rate) const
	{
		return (*m_startPos) * rate + (*m_endPos) * (1 - rate);
	}

	TerritoryVertex TerritoryEdge::GetStart() const
	{
		return m_startPos;
	}

	TerritoryVertex TerritoryEdge::GetEnd() const
	{
		return m_endPos;
	}

	bool TerritoryEdge::HasVertex(const TerritoryVertex& vertex) const
	{
		return m_startPos.get() == vertex.get() || m_endPos.get() == vertex.get();
	}

	Optional<EAngle> TerritoryEdge::GetDirectionOf(const TerritoryVertex& oneSideVertex) const
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

	void TerritoryEdge::MoveOnEdge(float* movingRate, EAngle direction, float speed)
	{
		if (direction == m_direction)
			*movingRate -= speed;
		else if (direction == Angle(m_direction).Reverse())
			*movingRate += speed;
		*movingRate = Util::RangeF(0, 1).Clamp(*movingRate);
	}
}
