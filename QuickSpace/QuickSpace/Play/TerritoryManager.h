#pragma once
#include "TerritoryEdge.h"
#include "QuickSpace/ActorBase.h"

namespace QuickSpace::Play
{
	class TerritoryManager : public ActorBase
	{
	public:
		void Init();
		void Update() override;
		Array<TerritoryEdge>& List();
	private:
		Array<TerritoryEdge> m_edgeList{};
	};
}
