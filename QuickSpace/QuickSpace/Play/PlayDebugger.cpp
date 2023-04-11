#include "stdafx.h"
#include "PlayDebugger.h"

#include "Player.h"
#include "PlayManager.h"

namespace QuickSpace::Play
{
	inline Vec2 headlineOffset()
	{
		return {0, 40};
	}

	void PlayDebugger::Update()
	{
		ActorBase::Update();

		checkTerrHistory();

		constexpr int buttonWidth = 80;
		constexpr int labelWidth = 80;
		constexpr int sliderWidth = 160;

		m_guiPosIndex = -1;

		// 領域経歴
		stepGuiPos();
		SimpleGUI::Headline(
			U"Territory History [{}/{}]"_fmt(m_terrHistoryIndex, m_terrHistory.size() - 1), getGuiPos());
		if (SimpleGUI::Button(U"Undo", getGuiPos() + headlineOffset(), buttonWidth))
		{
			undoTerrHistory();
		}
		if (SimpleGUI::Button(U"Redo", getGuiPos() + headlineOffset() + Vec2::Right(buttonWidth), buttonWidth))
		{
			redoTerrHistory();
		}

		// 領域詳細
		stepGuiPos();
		auto&&territory = PlayManager::Instance().Territory();
		SimpleGUI::Headline(
		U"Territory Edges: {}"_fmt(territory.Edges().size()), getGuiPos());
		SimpleGUI::Headline(
		U"Territory Frontier Edges: {}"_fmt(territory.Frontier().Edges().size()), getGuiPos() + headlineOffset());

		stepGuiPos();
		SimpleGUI::Headline(
		U"Territory Areas: {}"_fmt(territory.OccupiedAreas().size()), getGuiPos());
		// SimpleGUI::CheckBox(m_isTraceFrontierOutline, U"Trace Frontier", getGuiPos() + headlineOffset());
		// if (m_isTraceFrontierOutline) traceFrontierOutline(territory);

		// プレイヤー速度
		stepGuiPos();
		auto&& player = PlayManager::Instance().GetPlayer();
		double speedBase = player.MutSpeedBase();
		SimpleGUI::Headline(U"Player Speed", getGuiPos());
		SimpleGUI::Slider(
			U"{:.2f}"_fmt(speedBase),
			speedBase,
			0.05, 1.0,
			getGuiPos() + headlineOffset(),
			labelWidth, sliderWidth);
		player.MutSpeedBase() = static_cast<float>(speedBase);

		// プレイヤー移動
		stepGuiPos();
		SimpleGUI::Headline(U"Player Pos: {}"_fmt(player.EdgeCursor().asPoint()), getGuiPos());
		if (SimpleGUI::Button(U"Left", getGuiPos() + headlineOffset(), buttonWidth))
		{
			player.ForceMoveEdgeCursor(player.EdgeCursor() + Float2{-1, 0});
		}
		if (SimpleGUI::Button(U"Right", getGuiPos() + headlineOffset() + Vec2::Right(buttonWidth), buttonWidth))
		{
			player.ForceMoveEdgeCursor(player.EdgeCursor() + Float2{1, 0});
		}
		if (SimpleGUI::Button(U"Up", getGuiPos() + headlineOffset() + Vec2::Right(buttonWidth) * 2, buttonWidth))
		{
			player.ForceMoveEdgeCursor(player.EdgeCursor() + Float2{0, -1});
		}
		if (SimpleGUI::Button(U"Down", getGuiPos() + headlineOffset() + Vec2::Right(buttonWidth) * 3, buttonWidth))
		{
			player.ForceMoveEdgeCursor(player.EdgeCursor() + Float2{0, 1});
		}

	}

	float PlayDebugger::OrderPriority()
	{
		return 1000;
	}

	void PlayDebugger::checkTerrHistory()
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

	void PlayDebugger::stepGuiPos()
	{
		m_guiPosIndex++;
	}

	void PlayDebugger::undoTerrHistory()
	{
		if (m_terrHistory.size() == 0) return;
		m_terrHistoryIndex = std::max(0, m_terrHistoryIndex - 1);
		PlayManager::Instance().Territory().PasteState(m_terrHistory[m_terrHistoryIndex]);
		PlayManager::Instance().GetPlayer().Restart();
	}

	void PlayDebugger::redoTerrHistory()
	{
		if (m_terrHistory.size() == 0) return;
		m_terrHistoryIndex = std::min(static_cast<int>(m_terrHistory.size() - 1), m_terrHistoryIndex + 1);
		PlayManager::Instance().Territory().PasteState(m_terrHistory[m_terrHistoryIndex]);
		PlayManager::Instance().GetPlayer().Restart();
	}

	Vec2 PlayDebugger::getGuiPos()
	{
		return Vec2{40, 40 + 100 * m_guiPosIndex};
	}
}
