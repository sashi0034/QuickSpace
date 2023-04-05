﻿#pragma once
#include "SepEdge.h"

namespace QuickSpace::Play
{
	class SepFace
	{
	public:
		SepFace() = default;
		explicit SepFace(const Array<SepEdge>& edges);
		Optional<SepEdge> IntersectWith(const SepEdge& targetEdge);
	private:
		Array<SepEdge> m_edges{};
	};
}
