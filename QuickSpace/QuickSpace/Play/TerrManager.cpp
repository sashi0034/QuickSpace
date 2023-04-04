﻿#include "stdafx.h"
#include "TerrManager.h"

namespace QuickSpace::Play
{
	void TerrManager::Init()
	{
		const auto center = Scene::Center();

		constexpr int lineHalf = 480;

		const auto v00 = std::make_shared<Point>(center + Point(-lineHalf, -lineHalf));
		const auto v01 = std::make_shared<Point>(center + Point{-lineHalf, lineHalf});;
		const auto v10 = std::make_shared<Point>(center + Point{lineHalf, -lineHalf});;
		const auto v11 = std::make_shared<Point>(center + Point{lineHalf, lineHalf});;

		const auto e1 = std::make_shared<TerrEdge>(v00, v01);
		const auto e2 = std::make_shared<TerrEdge>(v01, v11);
		const auto e3 = std::make_shared<TerrEdge>(v11, v10);
		const auto e4 = std::make_shared<TerrEdge>(v10, v00);

		TerrEdge::ConnectEdges(e1, e2);
		TerrEdge::ConnectEdges(e2, e3);
		TerrEdge::ConnectEdges(e3, e4);
		TerrEdge::ConnectEdges(e4, e1);

		m_edgeList.push_back(e1);
		m_edgeList.push_back(e2);
		m_edgeList.push_back(e3);
		m_edgeList.push_back(e4);
	}

	void TerrManager::Update()
	{
		constexpr int lineWidth = 8;
		for (auto&& edge : m_edgeList)
		{
			Line{*edge->GetStart(), *edge->GetEnd()}.draw(lineWidth + 2, Color{32, 32,48});
		}
		for (auto&& edge : m_edgeList)
		{
			Line{*edge->GetStart(), *edge->GetEnd()}.draw(lineWidth, Color{160, 64, 196});
		}

		ActorBase::Update();
	}

	Array<TerrEdgeRef>& TerrManager::List()
	{
		return m_edgeList;
	}
}