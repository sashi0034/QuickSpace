#include "stdafx.h"
#include "PlayManager.h"

#include "PlayBg.h"
#include "Player.h"
#include "PlayDebugger.h"
#include "TerrManager.h"
#include "QuickSpace/ActorManager.h"
#include "QuickSpace/GameRoot.h"

namespace QuickSpace::Play
{
	PlayManager::PlayManager()
	{
		m_player = AsParent().BirthAs(new Player());
		m_playBg = AsParent().BirthAs(new PlayBg());
		m_territory = AsParent().BirthAs(new TerrManager());
#ifdef _DEBUG
		AsParent().Birth(new PlayDebugger());
#endif
	}

	void PlayManager::init()
	{
		// ゲームの前の各種初期化処理
		m_territory->Init();
		m_player->Init();
	}

	void PlayManager::StartPlay()
	{
		init();

		AsParent().StartCoro([&](auto&& yield){performPlay(yield);});
	}

	void PlayManager::Update()
	{
		ActorBase::Update();
	}

	Player& PlayManager::GetPlayer() const
	{
		return *m_player;
	}

	TerrManager& PlayManager::Territory() const
	{
		return *m_territory;
	}

	PlayManager& PlayManager::Instance()
	{
		return GameRoot::Global().GetPlayRoot();
	}

	CoroTask PlayManager::performPlay(CoroTaskYield& yield)
	{
		yield();
	}
}
