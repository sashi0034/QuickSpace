#include "stdafx.h"
#include "PlayerAction.h"

#include "QuickSpace/ConstParam.h"
#include "QuickSpace/GameAsset.h"
#include "QuickSpace/Util/Utils.h"

namespace QuickSpace::Play::PlayerAction
{
	void DrawPlayer(const Player& self)
	{
		const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

		const auto position = self.EdgeCursor();
		constexpr int cellSize = 32;
		constexpr int frameDuration = 250;
		const bool isWalking = self.IsMovedCursorNow();

		Point cellPos{};
		bool isMirrored = false;
		const float animValue = self.AnimValue();
		switch (self.GetAngle().Value())
		{
		case EAngle::Up:
			cellPos = isWalking
				? Point{Util::AnimFrameIndexF(animValue, 4, frameDuration), 5}
			: Point{Util::AnimFrameIndexF(animValue, 4, frameDuration), 2};
			break;
		case EAngle::Right:
			cellPos = isWalking
				? Point{Util::AnimFrameIndexF(animValue, 5, frameDuration), 4}
			: Point{Util::AnimFrameIndexF(animValue, 4, frameDuration), 1};
			break;
		case EAngle::Down:
			cellPos = isWalking
				? Point{Util::AnimFrameIndexF(animValue, 4, frameDuration), 3}
			: Point{Util::AnimFrameIndexF(animValue, 5, frameDuration), 0};
			break;
		case EAngle::Left:
			isMirrored = true;
			cellPos = isWalking
				? Point{Util::AnimFrameIndexF(animValue, 5, frameDuration), 4}
			: Point{Util::AnimFrameIndexF(animValue, 4, frameDuration), 1};
			break;
		default: ;
		}

		auto&& image =
			GameAsset::Instance().phine_32x32(cellPos.x * cellSize, cellPos.y * cellSize, cellSize, cellSize);
		if (isMirrored) image = image.mirrored();
		(void)image
			.scaled(ConstParam::PixelartScale)
			.drawAt(position - Vec2{0, ConstParam::PixelartScale * cellSize / 2});

	}
}
