#pragma once
#include "ActorManager.h"
#include "CoroutineManager.h"

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
		CoroutineManager& GetCoroutineManager();
	private:
		ActorManager m_actorManager;
		CoroutineManager m_coroutineManager;
		static std::unique_ptr<GameRoot> globalInstance;
	};
}
