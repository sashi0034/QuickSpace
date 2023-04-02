#include "stdafx.h"
#include "PlayManager.h"

namespace QuickSpace::Play
{
	void PlayManager::StartPlay()
	{
		m_isActive = true;
		CoroManager::Global().Start([&](auto&& yield){performPlay(yield);});
	}

	void PlayManager::Update()
	{
		if (m_isActive == false) return;

	}

	CoroTask PlayManager::performPlay(CoroTaskYield& yield)
	{
		yield();
	}
}
