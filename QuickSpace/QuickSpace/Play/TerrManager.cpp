#include "stdafx.h"
#include "TerrManager.h"

#include "Player.h"
#include "PlayManager.h"
#include "QuickSpace/ConstParam.h"
#include "QuickSpace/GameRoot.h"

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

		m_frontierFace = SepFace({SepEdge(e1), SepEdge(e2), SepEdge(e3), SepEdge(e4)});
	}

	void TerrManager::Update()
	{
		constexpr int lineWidth = ConstParam::LineWidth;
		for (auto&& edge : m_edgeList)
		{
			Line{*edge->GetStart(), *edge->GetEnd()}.draw(lineWidth + 2, Color{32, 32,48});
		}
		for (auto&& edge : m_edgeList)
		{
			auto color = Color{160, 64, 196};
			if (PlayManager::Instance().GetPlayer().GetEdgeTarget() == edge) color.b = 0;
			(void)Line{*edge->GetStart(), *edge->GetEnd()}.draw(lineWidth, color);
		}

		auto&& font = GameAsset::Instance().font16;
		for (auto&& edge : m_edgeList)
		{
			font(*edge->GetStart()).drawAt(*edge->GetStart() + Point::Up(12));
		}

		ActorBase::Update();
	}

	Array<TerrEdgeRef>& TerrManager::Edges()
	{
		return m_edgeList;
	}

	SepFace& TerrManager::Frontier()
	{
		return m_frontierFace;
	}

	void TerrManager::ResetFrontier(const SepFace& frontier)
	{
		m_frontierFace = frontier;
	}
}
