#include "stdafx.h"
#include "PlayManager.h"

#include "PlayBg.h"
#include "QuickSpace/ActorManager.h"

namespace QuickSpace::Play
{
	PlayManager::PlayManager()
	{
		m_playBg = AsParent().BirthAs(new PlayBg());
	}

	void PlayManager::StartPlay()
	{
		CoroManager::Global().Start([&](auto&& yield){performPlay(yield);});
	}

	void PlayManager::Update()
	{
		ActorBase::Update();
	}

	CoroTask PlayManager::performPlay(CoroTaskYield& yield)
	{
		yield();
	}
}
