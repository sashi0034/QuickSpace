#pragma once
#include "QuickSpace/Play/EnemyBase.h"

namespace QuickSpace::Play::En
{
	class EnKnightNormal final : public EnemyBase
	{
	public:
		void Init() override;
		void Update() override;
	private:

		void movePos();
		void drawImage() const;

		float m_moveSpeed = 90;
		float m_moveAngleSpeed = 120;
		float m_moveAngle{};
		float m_animValue{};
	};
}
