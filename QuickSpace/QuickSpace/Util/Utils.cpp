#include "stdafx.h"
#include "Utils.h"

namespace QuickSpace
{
	void Util::FillTexture(const Texture& texture, float tileSize, const Size& size, const Vec2& pos)
	{
		const ScopedRenderStates2D sampler{ SamplerState::RepeatLinear };
		const float scale = texture.size().x / tileSize;
		(void)texture.mapped(size * scale).scaled(1 / scale).draw(pos);
	}

	int Util::AnimFrameIndex(float controller, const int frameNumber, const int frameDuration)
	{
		return static_cast<int>(controller * 1000) / frameDuration % frameNumber;
	}
}
