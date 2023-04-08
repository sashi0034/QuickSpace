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
		// 点からベクトル(1, 1)への半直線の交差回数が偶数であれば、点は外部にあると判断
		bool isInside = false;
		for (auto&& edge : m_edges)
		{
			// 点が重なってたら終了
			if (edge.IsOverlappedVertex(point)) return false;

			if (edge.IsHorizontal())
			{
				// 水平かつ

				if ((point.y < edge.GetStart()->y) == false) continue;
				// 点のyが直線よりも下かつ

				const int intersectedX = point.x - point.y + edge.GetStart()->y; // 方程式の計算結果から

				if (Util::RangeInt::FromSort(edge.GetStart()->x, edge.GetEnd()->x)
					.ExtendRange(-1) // 交点が端点になるときは、垂直方向でチェックするようにする
					.IsBetween(intersectedX) == false) continue;
				// 交点のxが辺内にある

				// 反転
				isInside = !isInside;
			}
			else
			{
				// 垂直かつ

				if ((point.x < edge.GetStart()->x) == false) continue;
				// 点のyが直線よりも下かつ

				const int intersectedY = -point.x + point.y + edge.GetStart()->x; // 方程式の計算結果から

				if (Util::RangeInt::FromSort(edge.GetStart()->y, edge.GetEnd()->y)
					.IsBetween(intersectedY) == false) continue;
				// 交点のYが辺内にある

				// 反転
				isInside = !isInside;
			}
		}
		return isInside;
	}
}
