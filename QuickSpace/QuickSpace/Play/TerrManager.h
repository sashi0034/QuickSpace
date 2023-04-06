#pragma once
#include "SepFace.h"
#include "TerrEdge.h"
#include "QuickSpace/ActorBase.h"

namespace QuickSpace::Play
{
	class TerrManager : public ActorBase
	{
	public:
		void Init();
		void Update() override;
		Array<TerrEdgeRef>& Edges();
		SepFace& Frontier();
		void ResetFrontier(const SepFace& frontier);
	private:
		SepFace m_frontierFace{};
		Array<TerrEdgeRef> m_edgeList{};
	};
}
