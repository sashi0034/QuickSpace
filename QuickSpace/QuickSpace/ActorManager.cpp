#include "../stdafx.h"
#include "ActorManager.h"

#include "GameRoot.h"

namespace QuickSpace
{
	ActorManager& ActorManager::Global()
	{
		return GameRoot::Global().GetActorManager();
	}

	void ActorManager::sortActorList()
	{
		std::ranges::stable_sort(
			m_actorList,
			[](const std::shared_ptr<ActorBase>& left, const std::shared_ptr<ActorBase>& right)
			{
				return left->OrderPriority() < right->OrderPriority();
			});
	}

	void ActorManager::Update()
	{
		for (int i = m_actorList.size() - 1; i >= 0; --i)
		{
			auto&& actor = m_actorList[i];
			if (actor->IsDead()) m_actorList.erase(m_actorList.begin() + i);
		}

		// 優先度が高いほど後から更新するように並び変える
		sortActorList();

		// 更新
		for (int i = 0; i < m_actorList.size(); ++i)
		{
			auto&& actor = m_actorList[i];
			if (actor->IsActive() == false) continue;;
			actor->Update();
			// if (actor->HasChildren()) actor->AsParent().Update();
		}
	}

	void ActorManager::Clear()
	{
		m_actorList.clear();
	}

	void ActorManager::Birth(ActorBase* actor)
	{
		m_actorList.push_back(std::shared_ptr<ActorBase>(actor));
	}
}
