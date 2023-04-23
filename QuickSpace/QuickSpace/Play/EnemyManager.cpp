#include "stdafx.h"
#include "EnemyManager.h"

namespace QuickSpace::Play
{
	void EnemyManager::Update()
	{
		const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

		ActorBase::Update();
	}

	void EnemyManager::Init()
	{
		for (auto&& child : AsParent().ActorList())
		{
			if (auto&& en = std::dynamic_pointer_cast<EnemyBase>(child))
			{
				m_enemyList.push_back(en);
			}
		}
		for (auto&& enemy : m_enemyList)
		{
			enemy->Init();
		}
	}
}
