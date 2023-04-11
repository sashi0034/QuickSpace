#pragma once
#include "Angle.h"
#include "QuickSpace/Util/Range.h"

namespace QuickSpace::Play
{
	using TerrVertex = Point;

	class TerrEdge;
	using TerrEdgeRef = std::shared_ptr<TerrEdge>;

	struct TerrEdgeNeighbor
	{
		std::weak_ptr<TerrEdge> NeighborRef;
		TerrVertex OverlappedVertex;
	};

	class TerrEdge
	{
	public:
		TerrEdge();
		TerrEdge(const TerrEdge& origin);
		TerrEdge(const TerrVertex& startPos, const TerrVertex& endPos);
		[[nodiscard]] Vec2 Midpoint(float rate) const;
		[[nodiscard]] TerrVertex GetStart() const;
		[[nodiscard]] TerrVertex GetEnd() const;
		[[nodiscard]] int GetLength() const;
		void SetFixed(bool flag);
		bool IsFixed() const;
		void ChangeEnd(const TerrVertex& point);

		bool HasVertex(const TerrVertex& vertex) const;
		EAngle Direction() const;
		Optional<EAngle> GetDirectionFrom(const TerrVertex& oneSideVertex) const;
		bool IsOverlappedVertex(const TerrVertex& vertex) const;
		void MoveOnEdge(Float2* cursor, EAngle direction, float speed) const;
		void MoveOnEdgeByRate(float* movingRate, EAngle direction, float speed);
		bool IsHorizontal() const;
		bool IsNeighborWith(const TerrEdge& other);
		const Array<TerrEdgeNeighbor>& Neighbors() const;
		const Optional<TerrEdgeNeighbor> GetNearestNeighbor(const Float2& point, EAngle targetDirection) const;
		static void ConnectEdges(const TerrEdgeRef& neighbor1, const TerrEdgeRef& neighbor2);

		static EAngle CalcDirectionBetween(const TerrVertex& startPos, const TerrVertex& endPos);
		static bool IsOverlappedVertexBetween(const TerrVertex& startPos, const TerrVertex& endPos, const Point& checking);

		static Array<TerrEdgeRef> CopyTerrEdgesDeeply(const Array<TerrEdgeRef>& sourceArray);
	private:
		TerrVertex m_startPos{};
		TerrVertex m_endPos{};
		EAngle m_direction{};
		Array<TerrEdgeNeighbor> m_neighbors{};
		// プレイヤーが引っ張っているときオフ
		bool m_isFixed = true;

		void addNeighbor(const TerrEdgeRef& other);
		EAngle directionReversed() const;
	};
}
