#pragma once
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
		Point roundEdgeCursor() const;
		float getSpeed() const;

		float m_animValue{};

		void moveOnEdge();
		void checkFinishDrawing();
		void moveWithDraw();
		void startDrawing(EAngle angle);
		void checkMoveIntersect(EAngle angle, float speed, bool isHorizontal);
		static InputGroup& inputAngle(EAngle angle);
	};
}
