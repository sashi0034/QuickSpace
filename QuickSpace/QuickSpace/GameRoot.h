#pragma once
#include "ActorManager.h"
#include "CoroManager.h"
#include "GameAsset.h"

namespace QuickSpace
{
	class GameRoot
	{
	public:
		static GameRoot& Global();
		static void CreateGlobal();
		static void DestroyGlobal();

		void Init();
		void Update();

		ActorManager& GetActorManager();
		CoroManager& GetCoroutineManager();
		GameAsset& GetAsset();
	private:
		ActorManager m_actorManager{};
		CoroManager m_coroutineManager{};
		GameAsset m_asset{};
		static std::unique_ptr<GameRoot> globalInstance;
	};
}
