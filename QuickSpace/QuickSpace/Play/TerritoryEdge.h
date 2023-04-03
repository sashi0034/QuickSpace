#pragma once

namespace QuickSpace::Play
{
	using TerritoryVertex = s3d::Vec2;

	class TerritoryEdge
	{
	public:
		TerritoryEdge() = default;
		TerritoryEdge(const TerritoryVertex& startPos, const TerritoryVertex& endPos);
		TerritoryVertex Midpoint(float rate) const;
		[[nodiscard]] TerritoryVertex GetStart() const;
		[[nodiscard]] TerritoryVertex GetEnd() const;
	private:
		TerritoryVertex m_startPos{};
		TerritoryVertex m_endPos{};
	};
}
