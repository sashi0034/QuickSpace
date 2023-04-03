#pragma once
#include "TerrEdge.h"
#include "QuickSpace/ActorBase.h"

namespace QuickSpace::Play
{
	class TerrManager : public ActorBase
	{
	public:
		void Init();
		void Update() override;
		Array<TerrEdgeRef>& List();
	private:
		Array<TerrEdgeRef> m_edgeList{};
	};
}
