#pragma once
#include "../ActorBase.h"
#include "QuickSpace/CoroActor.h"

namespace QuickSpace::Play
{
	class PlayBg;
	class TerrManager;
	class Player;
	class EnemyManager;
	class StageBase;

	class PlayManager final : public ActorBase
	{
	public:
		PlayManager();
		void StartPlay();
		void Update() override;

		[[nodiscard]] Player& GetPlayer() const;
		[[nodiscard]] TerrManager& Territory() const;
		EnemyManager& Enemy() const;

		static PlayManager& Instance();
	private:
		std::shared_ptr<PlayBg> m_playBg;
		std::shared_ptr<Player> m_player;
		std::shared_ptr<TerrManager> m_territory;
		std::shared_ptr<EnemyManager> m_enemyManager;
		std::shared_ptr<StageBase> m_stageBase;

		void init();
		CoroTask performPlay(CoroTaskYield& yield);
	};

	// とりあえずグラフィックは今ので仮固定

}
