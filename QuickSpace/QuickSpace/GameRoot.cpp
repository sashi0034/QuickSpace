#include "../stdafx.h"
#include "GameRoot.h"

#include "Test/Demos.h"

namespace QuickSpace
{
	std::unique_ptr<GameRoot> GameRoot::globalInstance = std::make_unique<GameRoot>();

	GameRoot& GameRoot::Global()
	{
		return *globalInstance;
	}

	void GameRoot::EndGlobal()
	{
		globalInstance.reset(nullptr);
	}

	void GameRoot::Init()
	{
		Demos::InitDemos();
	}

	void GameRoot::Update()
	{
		m_actorManager.Update();
		m_coroutineManager.Update();
	}

	ActorManager& GameRoot::GetActorManager()
	{
		return m_actorManager;
	}

	CoroManager& GameRoot::GetCoroutineManager()
	{
		return m_coroutineManager;
	}
}
