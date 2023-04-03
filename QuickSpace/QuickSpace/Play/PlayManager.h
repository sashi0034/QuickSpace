#pragma once
#include "../ActorBase.h"
#include "QuickSpace/CoroManager.h"

namespace QuickSpace::Play
{
	class PlayBg;
	class TerrManager;
	class Player;

	class PlayManager final : public ActorBase
	{
	public:
		PlayManager();
		void StartPlay();
		void Update() override;

		[[nodiscard]] Player& GetPlayer() const;
		[[nodiscard]] TerrManager& Territory() const;

		static PlayManager& Instance();
	private:
		std::shared_ptr<PlayBg> m_playBg;
		std::shared_ptr<Player> m_player;
		std::shared_ptr<TerrManager> m_territory;

		void init();
		CoroTask performPlay(CoroTaskYield& yield);
	};

	// とりあえずグラフィックは今ので仮固定

}
