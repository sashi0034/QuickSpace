#include "stdafx.h"
#include "QuickSpace/GameRoot.h"

using namespace QuickSpace;

void Main()
{
	// 背景の色を設定する | Set the background color
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });

	GameRoot::CreateGlobal();
	GameRoot::Global().Init();

	while (System::Update())
	{
		GameRoot::Global().Update();
	}

	GameRoot::DestroyGlobal();
}
