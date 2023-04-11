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

		int m_guiPosIndex{};
		Array<TerrManagerState> m_terrHistory{};
		int m_terrHistoryIndex = 0;
	};
}
