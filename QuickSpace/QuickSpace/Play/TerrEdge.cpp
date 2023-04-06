﻿#include "stdafx.h"
#include "TerrEdge.h"

#include "QuickSpace/Util/Range.h"

namespace QuickSpace::Play
{
	TerrEdge::TerrEdge() = default;

	TerrEdge::TerrEdge(const TerrVertexRef& startPos, const TerrVertexRef& endPos) :
		m_startPos(startPos),
		m_endPos(endPos),
		m_direction(CalcDirectionBetween(startPos, endPos))
	{}

	Vec2 TerrEdge::Midpoint(float rate) const
	{
		return (*m_startPos) * rate + (*m_endPos) * (1 - rate);
	}

	TerrVertexRef TerrEdge::GetStart() const
	{
		return m_startPos;
	}

	TerrVertexRef TerrEdge::GetEnd() const
	{
		return m_endPos;
	}

	int TerrEdge::GetLength() const
	{
		return (*m_startPos - *m_endPos).manhattanLength();
	}

	void TerrEdge::SetFixed(bool flag)
	{
		m_isFixed = flag;
	}

	bool TerrEdge::IsFixed() const
	{
		return m_isFixed;
	}

	bool TerrEdge::HasVertex(const TerrVertexRef& vertex) const
	{
		return m_startPos.get() == vertex.get() || m_endPos.get() == vertex.get();
	}

	EAngle TerrEdge::Direction() const
	{
		return m_direction;
	}

	Optional<EAngle> TerrEdge::GetDirectionFrom(const TerrVertexRef& oneSideVertex) const
	{
		if (m_startPos.get() == oneSideVertex.get())
		{
			return m_direction;
		}
		if (m_endPos.get() == oneSideVertex.get())
		{
			return directionReversed();
		}
		return none;
	}

	bool TerrEdge::IsOverlappedVertex(const TerrVertexRef& vertex) const
	{
		return IsOverlappedVertexBetween(m_startPos, m_endPos, *vertex);
	}

	void TerrEdge::MoveOnEdge(Float2* cursor, EAngle direction, float speed) const
	{
		if (direction == m_direction || direction == directionReversed())
			*cursor += Angle(direction).ToFloat2() * speed;

		if (IsHorizontal())
		{
			cursor->y = m_startPos->y;
			cursor->x = Util::RangeInt::FromSort(m_startPos->x, m_endPos->x).Clamp(cursor->x);
		}
		else
		{
			cursor->x = m_startPos->x;
			cursor->y = Util::RangeInt::FromSort(m_startPos->y, m_endPos->y).Clamp(cursor->y);
		}
	}

	void TerrEdge::MoveOnEdgeByRate(float* movingRate, EAngle direction, float speed)
	{
		if (direction == m_direction)
			*movingRate -= speed;
		else if (direction == Angle(m_direction).Reverse())
			*movingRate += speed;
		*movingRate = Util::RangeF(0, 1).Clamp(*movingRate);
	}

	bool TerrEdge::IsHorizontal() const
	{
		return m_direction == EAngle::Left || m_direction == EAngle::Right;
	}

	bool TerrEdge::IsNeighborWith(const TerrEdge& other)
	{
		const bool isThisHorizontal = IsHorizontal();

		// 辺の向きが同じ
		if (isThisHorizontal == other.IsHorizontal())
		{
			return
				m_startPos.get() == other.m_startPos.get() || m_startPos.get() == other.m_endPos.get() ||
				m_endPos.get() == other.m_startPos.get() || m_endPos.get() == other.m_endPos.get();
		}

		// 辺の向きが違う
		if (isThisHorizontal)
			// 自身が水平
			return
				Util::RangeInt::FromSort(m_startPos->x, other.m_endPos->x).IsBetween(other.m_startPos->x) &&
				m_startPos->y == other.m_startPos->y && m_startPos->y == other.m_endPos->y;
		else
			// 自身が垂直
			return
				Util::RangeInt::FromSort(m_startPos->y, other.m_endPos->y).IsBetween(other.m_startPos->y) &&
				m_startPos->y == other.m_startPos->y && m_startPos->y == other.m_endPos->y;
	}

	const Array<TerrEdgeNeighbor>& TerrEdge::Neighbors() const
	{
		return m_neighbors;
	}

	const Optional<TerrEdgeNeighbor> TerrEdge::GetNearestNeighbor(const Float2& point, EAngle targetDirection) const
	{
		int resultIndex = -1;
		float resultDelta{};
		for (int i=0; i<m_neighbors.size(); ++i)
		{
			auto&& neighbor = m_neighbors[i];
			auto&& neighborRef = neighbor.NeighborRef.lock();
			if (neighborRef == nullptr) continue;

			// 隣接辺が始点または終点に重なっているときは特殊分岐
			if (neighborRef->m_startPos == neighbor.OverlappedVertex && neighborRef->m_direction != targetDirection) continue;
			if (neighborRef->m_endPos == neighbor.OverlappedVertex && neighborRef->directionReversed() != targetDirection) continue;
			// 隣接辺がその方向に伸びてるか水平か垂直かどうかで確認
			if (Angle(targetDirection).IsHorizontal() != neighborRef->IsHorizontal()) continue;

			float checkingDelta = checkingDelta = Math::Abs(IsHorizontal()
					? neighbor.OverlappedVertex->x - point.x
					: neighbor.OverlappedVertex->y - point.y);
			bool isUpdate = resultIndex == -1 || checkingDelta < resultDelta;
			if (isUpdate == false) continue;

			// 結果更新
			resultIndex = i;
			resultDelta = checkingDelta;
		}
		return resultIndex == -1 ? none : Optional(m_neighbors[resultIndex]);
	}

	void TerrEdge::ConnectEdges(const TerrEdgeRef& neighbor1, const TerrEdgeRef& neighbor2)
	{
		assert(neighbor1->IsHorizontal() != neighbor2->IsHorizontal());

		neighbor1->addNeighbor(neighbor2);
		neighbor2->addNeighbor(neighbor1);
	}

	EAngle TerrEdge::CalcDirectionBetween(const TerrVertexRef& startPos, const TerrVertexRef& endPos)
	{
		const bool isHorizontal = startPos->y == endPos->y;
		if (isHorizontal)
			return startPos->x < endPos->x ? EAngle::Right : EAngle::Left;
		else
			return startPos->y < endPos->y ? EAngle::Down : EAngle::Up;
	}

	bool TerrEdge::IsOverlappedVertexBetween(
		const TerrVertexRef& startPos, const TerrVertexRef& endPos, const Point& checking)
	{
		const bool isHorizontal = startPos->y == endPos->y;
		return isHorizontal
			? startPos->y == checking.y && Util::RangeInt::FromSort(startPos->x, endPos->x).IsBetween(checking.x)
			: startPos->x == checking.x && Util::RangeInt::FromSort(startPos->y, endPos->y).IsBetween(checking.y);
	}

	void TerrEdge::addNeighbor(const TerrEdgeRef& other)
	{
		TerrVertexRef overlappedVertex{};
		if (IsHorizontal())
		{
			if (m_startPos->x == other->m_startPos->x) overlappedVertex = m_startPos;
			else if (m_endPos->x == other->m_startPos->x) overlappedVertex = m_endPos;
			else if (m_startPos->y == other->m_startPos->y) overlappedVertex =  other->m_startPos;
			else if (m_startPos->y == other->m_endPos->y) overlappedVertex =  other->m_endPos;
		}
		else
		{
			if (m_startPos->y == other->m_startPos->y) overlappedVertex = m_startPos;
			if (m_endPos->y == other->m_startPos->y) overlappedVertex = m_endPos;
			else if (m_startPos->x == other->m_startPos->x) overlappedVertex =  other->m_startPos;
			else if (m_startPos->x == other->m_endPos->x) overlappedVertex =  other->m_endPos;
		}

		if (overlappedVertex.get() == nullptr)
		{
			assert(false);
			return;
		}

		m_neighbors.push_back(TerrEdgeNeighbor{other, overlappedVertex });
	}

	EAngle TerrEdge::directionReversed() const
	{
		return Angle(m_direction).Reverse();
	}
}
