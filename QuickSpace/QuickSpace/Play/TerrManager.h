#pragma once
#include "SepEdgeSet.h"
#include "SepFace.h"
#include "TerrEdge.h"
#include "QuickSpace/ActorBase.h"

namespace QuickSpace::Play
{
	struct TerrManagerState
	{
		SepFace FrontierFace{};
		Array<Polygon> OccupiedAreas{};
		Array<TerrEdgeRef> EdgeList{};
	};

	class TerrManager : public ActorBase
	{
	public:
		void Init();
		void Update() override;
		Array<TerrEdgeRef>& Edges();
		SepFace& Frontier();
		void ResetFrontier(const SepFace& frontier);
		void AddOccupiedArea(SepEdgeSet edgeSet);
		const Array<Polygon>& OccupiedAreas() const;
		[[nodiscard]] TerrManagerState CopyState() const;
		void PasteState(const TerrManagerState& state);
	private:
		struct AnimCb
		{
			float animRate = 0;
		};
		ConstantBuffer<AnimCb> m_animCb{};

		void drawOccupiedAreas();
		void drawTerritoryLines();

		TerrManagerState m_state{};
	};
}
