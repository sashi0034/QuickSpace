#include "stdafx.h"
#include "SepFace.h"

namespace QuickSpace::Play
{
	SepFace::SepFace(const Array<SepEdge>& edges)
	{
		m_edges = edges;
	}

	Optional<SepEdge> SepFace::IntersectWith(const SepEdge& targetEdge)
	{
		for (auto&& edge : m_edges)
		{
			if (edge.IsIntersectWith(targetEdge)) return edge;
		}
		return none;
	}
}
