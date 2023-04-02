#pragma once

#define REGISTER_GAMEASSET_PNG(fileName) const Texture fileName = loadPng(U ## #fileName);

namespace QuickSpace
{
	class GameAsset
	{
	public:
		static GameAsset& Instance();

		REGISTER_GAMEASSET_PNG(pondelion_32x32);
		REGISTER_GAMEASSET_PNG(bg_savannah);
		REGISTER_GAMEASSET_PNG(tex_savannah)
		// REGISTER_GAMEASSET_PNG(tex_savannah);

	private:
		Texture loadPng(String fileName);
	};
}
