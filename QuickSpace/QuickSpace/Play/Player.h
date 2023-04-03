﻿#pragma once
#include "TerrEdge.h"
#include "QuickSpace/ActorBase.h"

namespace QuickSpace::Play
{
	class Player : public ActorBase
	{
	public:
		Player();
		void Init();
		void Update() override;
		float OrderPriority() override;
	private:
		TerrEdgeRef m_edgeTarget{};
		float m_edgeProceededRate{};

		float m_animValue;

		void moveOnEdge();
	};
}
