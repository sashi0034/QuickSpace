#pragma once
#include "ActorManager.h"
#include "CoroManager.h"
#include "GameAsset.h"

namespace QuickSpace
{
	namespace Play
	{
		class PlayManager;
	}
	using namespace Play;

	class GameRoot
	{
	public:
		static GameRoot& Global();
		static void CreateGlobal();
		static void DestroyGlobal();

		GameRoot();

		void StartGame();
		void Update();

		ActorManager& GetActorManager();
		CoroManager& GetCoroutineManager();
		GameAsset& GetAsset();
		PlayManager& GetPlayRoot();
	private:
		ActorManager m_actorManager{};
		CoroManager m_coroutineManager{};
		GameAsset m_asset{};
		std::shared_ptr<PlayManager> m_playRoot{};

		CoroTask performGame(CoroTaskYield& yield);

		static std::unique_ptr<GameRoot> globalInstance;
	};
}
