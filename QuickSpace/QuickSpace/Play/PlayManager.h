﻿#pragma once
#include "../ActorBase.h"
#include "QuickSpace/CoroManager.h"

namespace QuickSpace::Play
{
	class PlayBg;
	class TerritoryManager;
	class Player;

	class PlayManager final : public ActorBase
	{
	public:
		PlayManager();
		void StartPlay();
		void Update() override;

		[[nodiscard]] Player& GetPlayer() const;
		[[nodiscard]] TerritoryManager& Territory() const;

		static PlayManager& Instance();
	private:
		std::shared_ptr<PlayBg> m_playBg;
		std::shared_ptr<Player> m_player;
		std::shared_ptr<TerritoryManager> m_territory;

		void init();
		CoroTask performPlay(CoroTaskYield& yield);
	};

	// とりあえずグラフィックは今ので仮固定

}
