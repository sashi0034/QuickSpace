#pragma once
#include "ActorManager.h"
#include "CoroActor.h"
#include "GameAsset.h"
#include "GameInput.h"

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
		GameAsset& GetAsset();
		PlayManager& GetPlayRoot();
		GameInput& GetInput();
	private:
		ActorManager m_actorManager{};
		GameAsset m_asset{};
		GameInput m_input{};
		std::shared_ptr<PlayManager> m_playRoot{};

		void testCase();
		CoroTask performGame(CoroTaskYield& yield);

		static std::unique_ptr<GameRoot> globalInstance;
	};
}
