#pragma once
#include "Angle.h"

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

		bool HasVertex(const TerrVertexRef& vertex) const;
		Optional<EAngle> GetDirectionOf(const TerrVertexRef& oneSideVertex) const;
		void MoveOnEdge(float* movingRate, EAngle direction, float speed);
		bool IsHorizontal() const;
		bool IsNeighborWith(const TerrEdge& other);
		const Array<TerrEdgeNeighbor>& Neighbors() const;
		static void ConnectEdges(const TerrEdgeRef& neighbor1, const TerrEdgeRef& neighbor2);
	private:
		TerrVertexRef m_startPos{};
		TerrVertexRef m_endPos{};
		EAngle m_direction{};
		Array<TerrEdgeNeighbor> m_neighbors{};

		void addNeighbor(const TerrEdgeRef& other);
	};
}
