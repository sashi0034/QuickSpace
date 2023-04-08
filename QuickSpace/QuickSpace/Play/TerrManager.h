#pragma once
#include "SepEdgeSet.h"
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
		void AddOccupiedArea(SepEdgeSet edgeSet);
	private:
		struct AnimCb
		{
			float animRate = 0;
		};
		ConstantBuffer<AnimCb> m_animCb{};

		void drawOccupiedAreas();

		SepFace m_frontierFace{};
		Array<Polygon> m_occupiedAreas{};
		Array<TerrEdgeRef> m_edgeList{};
	};
}
