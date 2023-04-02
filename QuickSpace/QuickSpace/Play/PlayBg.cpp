#include "stdafx.h"
#include "PlayBg.h"
#include "../Util.h"
#include "QuickSpace/GameAsset.h"

namespace QuickSpace::Play
{
	void PlayBg::Update()
	{
		// シーンのサイズぴったりにマッピングして描画
		Util::FillTexture(GameAsset::Instance().tex_savannah, 128.0f, Scene::Size(), Vec2{0, 0});

		ActorBase::Update();
	}
}
