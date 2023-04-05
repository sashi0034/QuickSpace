#pragma once
#include "SepEdge.h"
#include "TerrEdge.h"
#include "QuickSpace/ActorBase.h"

namespace QuickSpace::Play
{
	enum class EPlayerState
	{
		Moving,
		Drawing,
	};

	class Player : public ActorBase
	{
	public:
		Player();
		void Init();
		void Update() override;
		float OrderPriority() override;
		TerrEdgeRef& GetEdgeTarget();
	private:
		EPlayerState m_state = EPlayerState::Moving;
		TerrEdgeRef m_edgeTarget{};
		Float2 m_edgeCursor{};
		Array<SepEdge> m_drawnEdges{};
		Point roundEdgeCursor() const;
		float getSpeed() const;

		float m_animValue{};

		void moveOnEdge();
		void moveWithDraw();
		bool checkStartDrawing(EAngle angle);
		void startDrawing(EAngle angle);
		void continueDrawing(EAngle angle, const TerrVertexRef& oldEnd);
		void checkFinishDrawing();
		void confirmDrawingEdge();
		void extendDrawingEdge(EAngle direction);
		void checkMoveIntersect(EAngle angle, float speed, bool isHorizontal);
		static InputGroup& inputAngle(EAngle angle);
	};
}
