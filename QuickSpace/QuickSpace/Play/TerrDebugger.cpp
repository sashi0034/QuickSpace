#include "stdafx.h"
#include "TerrDebugger.h"

#include "Player.h"
#include "PlayManager.h"

namespace QuickSpace::Play
{
	inline Vec2 headlineOffset()
	{
		return {0, 40};
	}

	void TerrDebugger::Update()
	{
		ActorBase::Update();

		checkTerrHistory();

		constexpr int buttonWidth = 80;
		constexpr int labelWidth = 80;
		constexpr int sliderWidth = 160;

		SimpleGUI::Headline(
			U"Territory History [{}/{}]"_fmt(m_terrHistoryIndex, m_terrHistory.size() - 1), getGuiPos(0));
		if (SimpleGUI::Button(U"Undo", getGuiPos(0) + headlineOffset(), buttonWidth))
		{
			undoTerrHistory();
		}
		if (SimpleGUI::Button(U"Redo", getGuiPos(0) + headlineOffset() + Vec2::Right(buttonWidth), buttonWidth))
		{
			redoTerrHistory();
		}

		auto&& player = PlayManager::Instance().GetPlayer();
		double speedBase = player.MutSpeedBase();
		SimpleGUI::Headline(U"Player Speed", getGuiPos(1));
		SimpleGUI::Slider(
			U"{:.2f}"_fmt(speedBase),
			speedBase,
			0.1, 2.0,
			getGuiPos(1) + headlineOffset(),
			labelWidth, sliderWidth);
		player.MutSpeedBase() = static_cast<float>(speedBase);
	}

	float TerrDebugger::OrderPriority()
	{
		return 1000;
	}

	void TerrDebugger::checkTerrHistory()
	{
		auto&& territory =  PlayManager::Instance().Territory();
		const bool isUpdateHistory =
			m_terrHistory.size() == 0 ||
			m_terrHistory[m_terrHistoryIndex].OccupiedAreas.size() != territory.OccupiedAreas().size();
		if (isUpdateHistory == false) return;

		for (int i = m_terrHistory.size() - 1; i > m_terrHistoryIndex; --i)
		{
			m_terrHistory.pop_back();
		}
		m_terrHistory.push_back(territory.CopyState());
		m_terrHistoryIndex = m_terrHistory.size() - 1;
	}


	void TerrDebugger::undoTerrHistory()
	{
		if (m_terrHistory.size() == 0) return;
		m_terrHistoryIndex = std::max(0, m_terrHistoryIndex - 1);
		PlayManager::Instance().Territory().PasteState(m_terrHistory[m_terrHistoryIndex]);
		PlayManager::Instance().GetPlayer().Restart();
	}

	void TerrDebugger::redoTerrHistory()
	{
		if (m_terrHistory.size() == 0) return;
		m_terrHistoryIndex = std::min(static_cast<int>(m_terrHistory.size() - 1), m_terrHistoryIndex + 1);
		PlayManager::Instance().Territory().PasteState(m_terrHistory[m_terrHistoryIndex]);
		PlayManager::Instance().GetPlayer().Restart();
	}

	Vec2 TerrDebugger::getGuiPos(int column)
	{
		return Vec2{40, 40 + 100 * column};
	}
}
