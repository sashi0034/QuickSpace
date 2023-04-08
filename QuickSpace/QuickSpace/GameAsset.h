#pragma once

#define REGISTER_GAMEASSET_PNG(fileName) const Texture fileName = loadPng(U ## #fileName);

namespace QuickSpace
{
	class GameAsset
	{
	public:
		static GameAsset& Instance();

		REGISTER_GAMEASSET_PNG(phine_32x32);
		REGISTER_GAMEASSET_PNG(pondelion_32x32);
		REGISTER_GAMEASSET_PNG(bg_savannah);
		REGISTER_GAMEASSET_PNG(grass_tile_32x32);
		REGISTER_GAMEASSET_PNG(grass_tile_64x64)
		REGISTER_GAMEASSET_PNG(bush_16x16);
		REGISTER_GAMEASSET_PNG(tree_16x16);
		REGISTER_GAMEASSET_PNG(thin_weed_16x16);
		REGISTER_GAMEASSET_PNG(solid_rock_16x16);
		REGISTER_GAMEASSET_PNG(mixed_nature_16x16);
		// REGISTER_GAMEASSET_PNG(tex_savannah)

		Font font16{ 16 };
		Font font24{ 24 };
	private:
		Texture loadPng(String fileName);
	};
}
