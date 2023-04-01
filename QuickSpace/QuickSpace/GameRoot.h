#pragma once
#include "ActorManager.h"
#include "CoroManager.h"

namespace QuickSpace
{
	class GameRoot
	{
	public:
		static GameRoot& Global();
		static void EndGlobal();

		void Init();
		void Update();

		ActorManager& GetActorManager();
		CoroManager& GetCoroutineManager();
	private:
		ActorManager m_actorManager;
		CoroManager m_coroutineManager;
		static std::unique_ptr<GameRoot> globalInstance;
	};
}
