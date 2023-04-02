#pragma once
#include "../ActorBase.h"
#include "QuickSpace/CoroManager.h"

namespace QuickSpace::Play
{
	class PlayBg;

	class PlayManager final : public ActorBase
	{
	public:
		PlayManager();
		void StartPlay();
		void Update() override;
	private:
		std::shared_ptr<PlayBg> m_playBg;
		CoroTask performPlay(CoroTaskYield& yield);
	};

}
