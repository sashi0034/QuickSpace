#include "../stdafx.h"
#include "GameRoot.h"
#include "Play/PlayManager.h"
#include "Play/SepEdgeSet.h"
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

	GameRoot::GameRoot()
	{
		m_playRoot = m_actorManager.BirthAs(new PlayManager());
	}

	void GameRoot::StartGame()
	{
#if _DEBUG
		testCase();
#endif
		m_coroutineManager.Start([this](auto&& yield){performGame(yield);});
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

	PlayManager& GameRoot::GetPlayRoot()
	{
		return *m_playRoot;
	}

	GameInput& GameRoot::GetInput()
	{
		return m_input;
	}

	void GameRoot::testCase()
	{
		Play::SepEdgeSet::TestCase();
	}

	CoroTask GameRoot::performGame(CoroTaskYield& yield)
	{
		// Demos::InitDemos();
		m_playRoot->StartPlay();
		while (true)
		{
			yield();
		}
	}
}
