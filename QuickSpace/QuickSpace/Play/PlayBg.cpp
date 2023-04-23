#include "stdafx.h"
#include "PlayBg.h"

#include "QuickSpace/ConstParam.h"
#include "QuickSpace/CoroActor.h"
#include "QuickSpace/GameAsset.h"
#include "QuickSpace/Util/Utils.h"
using namespace AngelScript;

namespace QuickSpace::Play
{
	constexpr int chipSize = 16;
	inline int animFrameChip(int number, int duration)
	{
		return chipSize * Util::AnimFrameIndex(Time::GetMillisec(), number, duration);
	}

	void DrawBgChip(int32 x, int32 y, int32 kind)
	{

		constexpr int actualChipSize = chipSize * ConstParam::PixelartScale;
		const int scale = ConstParam::PixelartScale;
		const Vec2 pos = Scene::Center() + Vec2{x, y} * actualChipSize;

		auto&& asset = GameAsset::Instance();
		// 茂み1
		if (kind == 0) (void)asset.bush_16x16(animFrameChip(2, 500), 0, chipSize, chipSize).scaled(scale).draw(pos);
		// 茂み2
		else if (kind == 1) (void)asset.bush_16x16(animFrameChip(2, 500), chipSize, chipSize, chipSize).scaled(scale).draw(pos);
		// 石ころ
		else if (kind == 2) (void)asset.mixed_nature_16x16(0, 0, chipSize, chipSize).scaled(scale).draw(pos);
		// 岩
		else if (kind == 3) (void)asset.mixed_nature_16x16(chipSize, 0, chipSize, chipSize).scaled(scale).draw(pos);
		// コケ1
		else if (kind == 4) (void)asset.mixed_nature_16x16(0, chipSize, chipSize, chipSize).scaled(scale).draw(pos);
		// コケ2
		else if (kind == 5) (void)asset.mixed_nature_16x16(chipSize, chipSize, chipSize, chipSize).scaled(scale).draw(pos);
		// 固い岩
		else if (kind == 6) (void)asset.solid_rock_16x16(0, 0, chipSize, chipSize).scaled(scale).draw(pos);
		// 雑草
		else if (kind == 7) (void)asset.thin_weed_16x16(animFrameChip( 4, 200), 0, chipSize, chipSize).scaled(scale).draw(pos);
		// 木
		else if (kind == 8) (void)asset.tree_16x16(animFrameChip( 4, 250), 0, chipSize, chipSize).scaled(scale).draw(pos);
	}

	PlayBg::PlayBg()
	{
		m_script.GetEngine()->RegisterGlobalFunction(
			"void DrawBgChip(int32, int32, int32)",
			asFUNCTION(DrawBgChip),
			AngelScript::asCALL_CDECL);
		m_script.reload();
	}

	void PlayBg::Update()
	{
		const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

		// ベースのタイルを敷き詰める
		const int actualWidth = GameAsset::Instance().grass_tile_64x64.width() * ConstParam::PixelartScale;
		const int startX = Scene::Center().x - actualWidth * (Scene::Center().x / actualWidth + 1);
		const int startY = Scene::Center().y - actualWidth * (Scene::Center().y / actualWidth + 1);
		for (int x = startX; x < Scene::Size().x + actualWidth; x += actualWidth)
		{
			for (int y = startY; y < Scene::Size().y + actualWidth; y += actualWidth)
			{
				(void)GameAsset::Instance().grass_tile_64x64.scaled(ConstParam::PixelartScale).draw(x, y);
			}
		}

		updateScript();

		ActorBase::Update();
	}

	void PlayBg::updateScript()
	{
#ifdef _DEBUG
		if (m_scriptDirWatcher.retrieveChanges())
		{
			// リロード処理
			if (m_script.reload() == false)
			{
				AsParent().StartCoro([this](auto&& yield)
				{
					for (double time = 0; time < 5.0; time += Scene::DeltaTime())
					{
						if (m_script.getMessages().size() == 0) return;
						GameAsset::Instance().font24(m_script.getMessages().back()).drawAt(Scene::Center());
						yield();
					}
				});
			}
		}
#endif
		const auto scriptFunc = m_script.getFunction<void()>(U"DrawBg");
		String exception;
		scriptFunc.tryCall(exception);
		if (exception.isEmpty() == false) System::MessageBoxOK(exception);
	}
}
