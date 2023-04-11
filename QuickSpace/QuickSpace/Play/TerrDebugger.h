#pragma once
#include "TerrManager.h"
#include "QuickSpace/ActorBase.h"

namespace QuickSpace::Play
{
	class TerrDebugger : public ActorBase
	{
	public:
		void Update() override;
		float OrderPriority() override;
	private:
		void checkTerrHistory();
		Vec2 getGuiPos(int column);
		void undoTerrHistory();
		void redoTerrHistory();

		Array<TerrManagerState> m_terrHistory{};
		int m_terrHistoryIndex = 0;
	};
}
