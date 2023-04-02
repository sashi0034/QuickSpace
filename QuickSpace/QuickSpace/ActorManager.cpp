#include "../stdafx.h"
#include "ActorManager.h"

#include "GameRoot.h"

namespace QuickSpace
{
	ActorManager& ActorManager::Global()
	{
		return GameRoot::Global().GetActorManager();
	}


	// 子から親に追従
	void ActorManager::followActorParent(std::shared_ptr<ActorBase>& actor)
	{
		if (auto parent = actor->GetParent().lock())
		{
			if (parent->Internal().HasFollowedParent == false) followActorParent(parent);

			if (parent->IsDead()) actor->Kill();
			actor->Internal().IsActiveInHierarchy = parent->Internal().IsActiveInHierarchy;
		}
		else
		{
			actor->Internal().IsActiveInHierarchy = actor->IsActive();
		}
		actor->Internal().HasFollowedParent = true;
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
		// この状態を親に同期
		for (auto&& actor : m_actorList)
		{
			if (actor->Internal().HasFollowedParent) continue;
			followActorParent(actor);
		}

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
			if (actor->Internal().IsActiveInHierarchy) actor->Update();
			actor->Internal().IsActiveInHierarchy = false;
			actor->Internal().HasFollowedParent = false;
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
