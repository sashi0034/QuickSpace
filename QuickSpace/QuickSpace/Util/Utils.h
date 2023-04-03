#pragma once

namespace QuickSpace
{
	namespace Util
	{
		void FillTexture(const Texture& texture, float tileSize, const Size& size, const Vec2& pos);
		int AnimFrameIndex(float controller, const int frameNumber, const int frameDuration);
	}
}
