#pragma once
#include "TerrEdge.h"
#include "QuickSpace/ActorBase.h"

namespace QuickSpace::Play
{
	class Player : public ActorBase
	{
	public:
		Player();
		void Init();
		void Update() override;
		float OrderPriority() override;
	private:
		TerrEdgeRef m_edgeTarget{};
		Float2 m_edgeCursor{};

		float m_animValue{};

		void moveOnEdge();
		void checkMoveToNeighbor(EAngle angle, float speed, bool isHorizontal);
	};
}
