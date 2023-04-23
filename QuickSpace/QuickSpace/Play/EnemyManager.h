#pragma once
#include "EnemyBase.h"
#include "QuickSpace/ActorBase.h"

namespace QuickSpace::Play
{
	class EnemyManager : public ActorBase
	{
	public:
		void Update() override;
		void Init();
	private:
		Array<std::shared_ptr<EnemyBase>> m_enemyList{};
	};
}
