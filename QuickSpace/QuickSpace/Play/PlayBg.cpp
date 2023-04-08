#include "stdafx.h"
#include "PlayBg.h"

#include "QuickSpace/ConstParam.h"
#include "QuickSpace/GameAsset.h"
#include "QuickSpace/Util/Utils.h"

namespace QuickSpace::Play
{
	void PlayBg::Update()
	{
		// const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

		const auto size = Scene::Size();
		// TODO: 別の方法で
		Util::FillTexture(
			GameAsset::Instance().grass_tile_64x64,
			GameAsset::Instance().grass_tile_64x64.width() * ConstParam::PixelartScale,
			size,
			Scene::Center() - size / 2);

		ActorBase::Update();
	}
}
