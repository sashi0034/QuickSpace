#include "../stdafx.h"
#include "GameRoot.h"

#include "Test/Demos.h"

namespace QuickSpace
{
	std::unique_ptr<GameRoot> GameRoot::globalInstance{};

	GameRoot& GameRoot::Global()
	{
		return *globalInstance;
	}

	void GameRoot::CreateGlobal()
	{
		globalInstance.reset(new GameRoot());
	}

	void GameRoot::DestroyGlobal()
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

	GameAsset& GameRoot::GetAsset()
	{
		return m_asset;
	}
}
