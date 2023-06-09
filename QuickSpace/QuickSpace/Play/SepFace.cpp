﻿#include "stdafx.h"
#include "SepFace.h"

#include "SepEdgeSet.h"

namespace QuickSpace::Play
{
	void SepFace::TestCase()
	{
		// 以下の形状の図形を想定
		// ----------
		// |        |
		// -----    -----
		//     |        |
		//     ----------

		auto face = CreateClockwiseFace(SepEdgeSet::CreateFromVertexes(Array{
			TerrVertex{100, 100},
			TerrVertex{300, 100},
			TerrVertex{300, 200},
			TerrVertex{400, 200},
			TerrVertex{400, 300},
			TerrVertex{200, 300},
			TerrVertex{200, 200},
			TerrVertex{100, 200},
		}).Edges());

		assert(face.JudgePointInside(Point{150, 75}) == false);
		assert(face.JudgePointInside(Point{125, 150}) == true);
		assert(face.JudgePointInside(Point{0, 0}) == false);
		assert(face.JudgePointInside(Point{150, 150}) == true);
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

	Optional<bool> SepFace::JudgePointInside(const Point& point)
	{
		for (int i=0; i<static_cast<int>(JudgingLine::Max); ++i)
		{
			auto result = judgePointInsidePartial(point, static_cast<JudgingLine>(i));
			if (result != none) return result;
		}
		return none;
	}

	Optional<bool> SepFace::judgePointInsidePartial(const Point& point, JudgingLine judgingLine)
	{
		// 点からベクトル(1, 1)などへの半直線の交差回数が偶数であれば、点は外部にあると判断
		bool isInside = false;
		for (auto&& edge : m_edges)
		{
			// 点が重なってたら終了
			if (edge.IsOverlappedVertex(point)) return false;

			if (edge.IsHorizontal())
			{
				// 水平かつ

				if (judgingLine == Right_Down || judgingLine == Left_Down
					? (point.y < edge.GetStart().y) == false
					: (point.y > edge.GetStart().y) == false) continue;
				// 点のyが直線よりも下などかつ

				const int intersectedX = judgingLine == Right_Down || judgingLine == Left_Up
					// 方程式の計算結果から
					? point.x - point.y + edge.GetStart().y
					: point.x + point.y - edge.GetStart().y;

				// 端点と交わるときは判定不能扱いにする
				if (edge.GetStart().x == intersectedX ||
					edge.GetEnd().x == intersectedX) return none;

				if (Util::RangeInt::FromSort(edge.GetStart().x, edge.GetEnd().x)
					.IsBetween(intersectedX) == false) continue;
				// 交点のxが辺内にある

				// 反転
				isInside = !isInside;
			}
			else
			{
				// 垂直かつ

				if (judgingLine == Right_Down || judgingLine == Right_Up
					? (point.x < edge.GetStart().x) == false
					: (point.x > edge.GetStart().x) == false) continue;
				// 点のxが直線よりも左などかつ

				const int intersectedY = judgingLine == Right_Down || judgingLine == Left_Up
					// 方程式の計算結果から
					? -point.x + point.y + edge.GetStart().x
					: point.x + point.y - edge.GetStart().x;

				// 端点と交わるときは判定不能扱いにする
				if (edge.GetStart().y == intersectedY ||
					edge.GetEnd().y == intersectedY) return none;

				if (Util::RangeInt::FromSort(edge.GetStart().y, edge.GetEnd().y)
					.IsBetween(intersectedY) == false) continue;
				// 交点のYが辺内にある

				// 反転
				isInside = !isInside;
			}
		}
		return isInside;
	}

	SepFace SepFace::CreateClockwiseFace(SepEdgeSet& edgeSet)
	{
		return CreateClockwiseFace(edgeSet.Edges());
	}

	SepFace SepFace::CreateClockwiseFace(const Array<SepEdge>& edges)
	{
		auto clockwise = SepEdgeSet(edges).CreateClockwiseCircuit();

		auto result = SepFace();
		result.m_edges = std::move(clockwise.Edges());

		return result;
	}
}
