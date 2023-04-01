#include "../stdafx.h"
#include "GameAsset.h"

#include "GameRoot.h"

QuickSpace::GameAsset& QuickSpace::GameAsset::Instance()
{
	return GameRoot::Global().GetAsset();
}

Texture QuickSpace::GameAsset::loadPng(String fileName)
{
	const String path = U"asset/image/{}.png"_fmt(fileName);
	return Texture{path};
}
