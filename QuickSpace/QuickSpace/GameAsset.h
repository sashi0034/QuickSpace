#pragma once

#define REGISTER_GAMEASSET_PNG(fileName) const Texture fileName = loadPng(U ## #fileName);

namespace QuickSpace
{
	class GameAsset
	{
	public:
		static GameAsset& Instance();

		REGISTER_GAMEASSET_PNG(pondelion_32x32);

	private:
		Texture loadPng(String fileName);
	};
}
