#include "stdafx.h"
#include "Stage0.h"

#include "QuickSpace/Play/EnemyManager.h"
#include "QuickSpace/Play/PlayManager.h"
#include "QuickSpace/Play/En/EnKnightNormal.h"

namespace QuickSpace::Play::Stage
{
	void Stage0::Setup()
	{
		StageBase::Setup();

		PlayManager::Instance().Enemy().AsParent().Birth(
			&(new En::EnKnightNormal())->SetPos(Scene::Center()));

	}
}
