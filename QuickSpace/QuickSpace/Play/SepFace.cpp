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

	Array<SepEdge>& SepFace::Edges()
	{
		return m_edges;
	}

	bool SepFace::IsPointInside(const Point& point)
	{
		// 点から真上への半直線の交差回数が偶数であれば、点は外部にあると判断
		bool isInside = false;
		for (auto&& edge : m_edges)
		{
			// 点が重なってたら終了
			if (edge.IsOverlappedVertex(point)) return false;

			if (edge.IsHorizontal())
			{
				// 水平かつ

				if ((point.y > edge.GetStart()->y) == false) continue;
				// 点のyが直線よりも下かつ

				if (Util::RangeInt::FromSort(edge.GetStart()->x, edge.GetEnd()->x)
					// .ExtendRange(-1)
					.IsBetween(point.x) == false) continue;
				// 点のxが直線内にある

				// 反転
				isInside = !isInside;
			}
			else
			{
				// 垂直かつ

				if ((point.x == edge.GetStart()->x) == false) continue;
				// 点とxが同じかつ

				if ((point.y > edge.GetStart()->y) == false) continue;
				// 点のyが直線よりも下なら

				// 反転
				isInside = !isInside;
			}
		}
		return isInside;
	}
}
