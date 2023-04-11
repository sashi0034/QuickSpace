#pragma once
#include "TerrManager.h"
#include "QuickSpace/ActorBase.h"

namespace QuickSpace::Play
{
	class PlayDebugger : public ActorBase
	{
	public:
		void Update() override;
		float OrderPriority() override;
	private:
		void checkTerrHistory();
		void stepGuiPos();
		Vec2 getGuiPos();
		void undoTerrHistory();
		void redoTerrHistory();
		void traceFrontierOutline(TerrManager& territory);

		int m_guiPosIndex{};
		Array<TerrManagerState> m_terrHistory{};
		int m_terrHistoryIndex = 0;
		bool m_isTraceFrontierOutline = true;
	};
}
