#pragma once
#include "../ActorBase.h"
#include "QuickSpace/CoroManager.h"

namespace QuickSpace::Play
{
	class PlayManager final : ActorBase
	{
	public:
		void StartPlay();
		void Update() override;
	private:
		bool m_isActive = false;
		CoroTask performPlay(CoroTaskYield& yield);
	};

}
