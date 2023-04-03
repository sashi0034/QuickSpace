#pragma once
#include "Angle.h"

namespace QuickSpace::Play
{
	using TerritoryVertex = std::shared_ptr<Vec2>;

	class TerritoryEdge
	{
	public:
		TerritoryEdge();
		TerritoryEdge(const TerritoryVertex& startPos, const TerritoryVertex& endPos);
		[[nodiscard]] Vec2 Midpoint(float rate) const;
		[[nodiscard]] TerritoryVertex GetStart() const;
		[[nodiscard]] TerritoryVertex GetEnd() const;

		bool HasVertex(const TerritoryVertex& vertex) const;
		Optional<EAngle> GetDirectionOf(const TerritoryVertex& oneSideVertex) const;
		void MoveOnEdge(float* movingRate, EAngle direction, float speed);
	private:
		TerritoryVertex m_startPos{};
		TerritoryVertex m_endPos{};
		EAngle m_direction{};
	};
}
