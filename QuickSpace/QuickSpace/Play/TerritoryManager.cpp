#include "stdafx.h"
#include "TerritoryManager.h"

namespace QuickSpace::Play
{
	void TerritoryManager::Init()
	{
		const auto center = Scene::Center();

		constexpr double lineHalf = 480;

		const auto v00 = std::make_shared<Vec2>(center + Vec2{-lineHalf, -lineHalf});
		const auto v01 = std::make_shared<Vec2>(center + Vec2{-lineHalf, lineHalf});;
		const auto v10 = std::make_shared<Vec2>(center + Vec2{lineHalf, -lineHalf});;
		const auto v11 = std::make_shared<Vec2>(center + Vec2{lineHalf, lineHalf});;

		m_edgeList.push_back(TerritoryEdge(v00, v01));
		m_edgeList.push_back(TerritoryEdge(v01, v11));
		m_edgeList.push_back(TerritoryEdge(v11, v10));
		m_edgeList.push_back(TerritoryEdge(v10, v00));
	}

	void TerritoryManager::Update()
	{
		for (auto&& edge : m_edgeList)
		{
			constexpr int width = 10;
			Line{*edge.GetStart(), *edge.GetEnd()}.draw(width + 2, Color{32, 32,48});
		}
		for (auto&& edge : m_edgeList)
		{
			constexpr int width = 10;
			Line{*edge.GetStart(), *edge.GetEnd()}.draw(width, Color{160, 64, 196});
		}

		ActorBase::Update();
	}

	Array<TerritoryEdge>& TerritoryManager::List()
	{
		return m_edgeList;
	}
}
