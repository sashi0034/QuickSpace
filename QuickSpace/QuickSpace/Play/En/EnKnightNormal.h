#pragma once
#include "QuickSpace/Play/EnemyBase.h"

namespace QuickSpace::Play::En
{
	class EnKnightNormal final : public EnemyBase
	{
	public:
		void Init() override;
		void Update() override;
	};
}
