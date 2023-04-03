#include "stdafx.h"
#include "TerritoryEdge.h"

namespace QuickSpace::Play
{
	TerritoryEdge::TerritoryEdge(const TerritoryVertex& startPos, const TerritoryVertex& endPos) :
		m_startPos(startPos),
		m_endPos(endPos)
	{}

	TerritoryVertex TerritoryEdge::Midpoint(float rate) const
	{
		return m_endPos * rate + m_endPos * (1 - rate);
	}

	TerritoryVertex TerritoryEdge::GetStart() const
	{
		return m_startPos;
	}

	TerritoryVertex TerritoryEdge::GetEnd() const
	{
		return m_endPos;
	}
}
