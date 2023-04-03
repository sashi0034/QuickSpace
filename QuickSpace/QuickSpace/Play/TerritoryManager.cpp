#include "stdafx.h"
#include "TerritoryManager.h"

namespace QuickSpace::Play
{
	void TerritoryManager::Init()
	{
		const auto center = Scene::Center();

		constexpr double lineHalf = 480;

		m_edgeList.push_back(
			TerritoryEdge(center + Vec2{-lineHalf, -lineHalf}, center + Vec2{-lineHalf, lineHalf}));
		m_edgeList.push_back(
			TerritoryEdge(center + Vec2{-lineHalf, lineHalf}, center + Vec2{lineHalf, lineHalf}));
		m_edgeList.push_back(
			TerritoryEdge(center + Vec2{lineHalf, lineHalf}, center + Vec2{lineHalf, -lineHalf}));
		m_edgeList.push_back(
			TerritoryEdge(center + Vec2{lineHalf, -lineHalf}, center + Vec2{-lineHalf, -lineHalf}));
	}

	void TerritoryManager::Update()
	{
		for (auto&& edge : m_edgeList)
		{
			constexpr int width = 10;
			Line{edge.GetStart(), edge.GetEnd()}.draw(width + 2, Color{32, 32,48});
		}
		for (auto&& edge : m_edgeList)
		{
			constexpr int width = 10;
			Line{edge.GetStart(), edge.GetEnd()}.draw(width, Color{160, 64, 196});
		}

		ActorBase::Update();
	}

	Array<TerritoryEdge>& TerritoryManager::List()
	{
		return m_edgeList;
	}
}
