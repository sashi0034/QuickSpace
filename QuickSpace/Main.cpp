#include "stdafx.h"
#include "QuickSpace/GameRoot.h"

using namespace QuickSpace;

void Main()
{
	Window::SetStyle(WindowStyle::Sizable);
	Scene::SetResizeMode(ResizeMode::Keep);
	const Size sceneSize = {1920, 1080};
	Scene::Resize(sceneSize.x, sceneSize.y);
	Window::Resize(1280, 720);
	Scene::SetBackground(ColorF{ 0.3, 0.3, 0.3 });

	GameRoot::CreateGlobal();
	GameRoot::Global().StartGame();

	while (System::Update())
	{
		GameRoot::Global().Update();
	}

	GameRoot::DestroyGlobal();
}
