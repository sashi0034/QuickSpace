#pragma once
#include "SepEdge.h"

namespace QuickSpace::Play
{
	class SepFace
	{
	public:
		SepFace() = default;
		explicit SepFace(const Array<SepEdge>& edges);
		Optional<SepEdge> IntersectWith(const SepEdge& targetEdge);
		Array<SepEdge>& Edges();
		Optional<bool> JudgePointInside(const Point& point);
	private:
		Array<SepEdge> m_edges{};
	};
}
