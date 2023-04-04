﻿#pragma once
#include "Angle.h"
#include "QuickSpace/Util/Range.h"

namespace QuickSpace::Play
{
	using TerrVertexRef = std::shared_ptr<Point>;

	class TerrEdge;
	using TerrEdgeRef = std::shared_ptr<TerrEdge>;

	struct TerrEdgeNeighbor
	{
		std::weak_ptr<TerrEdge> NeighborRef;
		TerrVertexRef OverlappedVertex;
	};

	class TerrEdge
	{
	public:
		TerrEdge();
		TerrEdge(const TerrVertexRef& startPos, const TerrVertexRef& endPos);
		[[nodiscard]] Vec2 Midpoint(float rate) const;
		[[nodiscard]] TerrVertexRef GetStart() const;
		[[nodiscard]] TerrVertexRef GetEnd() const;

		int DirFixedVal() const;
		Util::RangeInt DirRangeVal() const;

		bool HasVertex(const TerrVertexRef& vertex) const;
		Optional<EAngle> GetDirectionOf(const TerrVertexRef& oneSideVertex) const;
		void MoveOnEdge(Float2* cursor, EAngle direction, float speed) const;
		void MoveOnEdgeByRate(float* movingRate, EAngle direction, float speed);
		bool IsHorizontal() const;
		bool IsNeighborWith(const TerrEdge& other);
		const Array<TerrEdgeNeighbor>& Neighbors() const;
		const Optional<TerrEdgeNeighbor> GetNearestNeighbor(const Float2& point, EAngle targetDirection) const;
		static void ConnectEdges(const TerrEdgeRef& neighbor1, const TerrEdgeRef& neighbor2);
	private:
		TerrVertexRef m_startPos{};
		TerrVertexRef m_endPos{};
		EAngle m_direction{};
		Array<TerrEdgeNeighbor> m_neighbors{};

		void addNeighbor(const TerrEdgeRef& other);
		EAngle directionReversed() const;
	};
}