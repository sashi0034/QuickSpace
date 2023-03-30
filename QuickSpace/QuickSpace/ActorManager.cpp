﻿#include "../stdafx.h"
#include "ActorManager.h"

namespace QuickSpace
{
	ActorManager ActorManager::globalInstance = ActorManager{};

	ActorManager& ActorManager::Global()
	{
		return globalInstance;
	}

	void ActorManager::Update()
	{
		for (auto&& actor : m_actorList)
		{
			actor->Update();
		}
	}

	void ActorManager::Clear()
	{
		m_actorList.clear();
	}

	void ActorManager::Birth(IActor* actor)
	{
		m_actorList.push_back(std::shared_ptr<IActor>(actor));
	}
}