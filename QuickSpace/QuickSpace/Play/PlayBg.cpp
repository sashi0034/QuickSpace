#include "stdafx.h"
#include "PlayBg.h"
#include "../Util.h"
#include "QuickSpace/GameAsset.h"

namespace QuickSpace::Play
{
	void PlayBg::Update()
	{
		GameAsset::Instance().bg_savannah.resized(Scene::Size()).drawAt(Scene::Center());

		// auto size = Size{960, 960};
		// Util::FillTexture(GameAsset::Instance().tex_savannah, (size / 2).x, size, Scene::Center() - size / 2);

		ActorBase::Update();
	}
}
