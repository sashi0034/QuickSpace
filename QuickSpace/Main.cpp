#include "stdafx.h"
#include "QuickSpace/ActorManager.h"
#include "QuickSpace/Demos.h"

void Main()
{
	// 背景の色を設定する | Set the background color
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });

	QuickSpace::Demos::InitDemos();

	while (System::Update())
	{
		QuickSpace::ActorManager::Global().Update();
	}

	QuickSpace::ActorManager::Global().Clear();
}
