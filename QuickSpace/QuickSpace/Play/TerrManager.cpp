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

		const auto v00 = Point(center + Point(-lineHalf, -lineHalf));
		const auto v01 = Point(center + Point{-lineHalf, lineHalf});;
		const auto v10 = Point(center + Point{lineHalf, -lineHalf});;
		const auto v11 = Point(center + Point{lineHalf, lineHalf});;

		const auto e1 = std::make_shared<TerrEdge>(v00, v01);
		const auto e2 = std::make_shared<TerrEdge>(v01, v11);
		const auto e3 = std::make_shared<TerrEdge>(v11, v10);
		const auto e4 = std::make_shared<TerrEdge>(v10, v00);

		TerrEdge::ConnectEdges(e1, e2);
		TerrEdge::ConnectEdges(e2, e3);
		TerrEdge::ConnectEdges(e3, e4);
		TerrEdge::ConnectEdges(e4, e1);

		m_state.EdgeList.push_back(e1);
		m_state.EdgeList.push_back(e2);
		m_state.EdgeList.push_back(e3);
		m_state.EdgeList.push_back(e4);

		m_state.FrontierFace = SepFace({SepEdge(e1), SepEdge(e2), SepEdge(e3), SepEdge(e4)});
	}

	void TerrManager::Update()
	{
		constexpr float animSpeed = 0.3f;
		m_animCb->animRate = m_animCb->animRate + Scene::DeltaTime() * animSpeed;

		drawOccupiedAreas();

		// 線描画
		drawTerritoryLines();

		auto&& font = GameAsset::Instance().font16;
		for (auto&& edge : m_state.EdgeList)
		{
			font(edge->GetStart()).drawAt(edge->GetStart() + Point::Up(12));
			font(edge->GetEnd()).drawAt(edge->GetEnd() + Point::Up(12));
		}

		ActorBase::Update();
	}

	void TerrManager::drawOccupiedAreas()
	{
		Graphics2D::SetPSConstantBuffer(1, m_animCb);

		Graphics2D::SetPSTexture(1, GameAsset::Instance().tex_aqua_noise);
		Graphics2D::SetPSTexture(2, GameAsset::Instance().tex_cosmos_noise);

		const ScopedCustomShader2D shader{ GameAsset::Instance().psFantasyPolygon };

		// 面描画
		for (auto&& area : m_state.OccupiedAreas)
		{
			auto color = Color{220, 64, 240, 224};
			(void)area.draw(color);
		}
	}

	void TerrManager::drawTerritoryLines()
	{
		const float animAmp = Math::Sin(m_animCb->animRate * Math::Pi * 2.5f);
		const int lineWidth =
			ConstParam::LineWidth - static_cast<int>(4.5f * Math::Abs(animAmp));
		for (auto&& edge : m_state.EdgeList)
		{
			// 影
			Line{edge->GetStart(), edge->GetEnd()}.draw(lineWidth + 2, Color{240, 122, 255});
		}

		// プレイヤーが書き込んでいる最中の辺
		for (auto&& edge : m_state.EdgeList)
		{
			if (edge->IsFixed()) continue;
			const float brightness = 1 + (1 - Math::Abs(animAmp)) * 0.3f;
			auto color = ColorF{ 47 / 255.0f, 101 / 255.0f, 221 / 255.0f } * brightness;
			if (PlayManager::Instance().GetPlayer().GetEdgeTarget() == edge)
				color *= 1.3f;
			(void)Line{edge->GetStart(), edge->GetEnd()}.draw(lineWidth, color);
		}

		// 固定された辺はカスタムシェーダで描画
		Graphics2D::SetPSConstantBuffer(1, m_animCb);
		const ScopedCustomShader2D shader{ GameAsset::Instance().psFantasyLine };

		for (auto&& edge : m_state.EdgeList)
		{
			if (edge->IsFixed() == false) continue;
			const float brightness = 1 - (1 - Math::Abs(animAmp)) * 0.1f;
			auto color = ColorF{144 / 240.0f, 64 / 255.0f, 176 / 255.0f} * brightness;
			if (PlayManager::Instance().GetPlayer().GetEdgeTarget() == edge)
				color.b = 1.0f;
			(void)Line{edge->GetStart(), edge->GetEnd()}.draw(lineWidth, color);
		}
	}

	Array<TerrEdgeRef>& TerrManager::Edges()
	{
		return m_state.EdgeList;
	}

	SepFace& TerrManager::Frontier()
	{
		return m_state.FrontierFace;
	}

	void TerrManager::ResetFrontier(const SepFace& frontier)
	{
		m_state.FrontierFace = frontier;
	}

	void TerrManager::AddOccupiedArea(SepEdgeSet edgeSet)
	{
		m_state.OccupiedAreas.push_back(edgeSet.ConstructPolygon());
	}

	const Array<Polygon>& TerrManager::OccupiedAreas() const
	{
		return m_state.OccupiedAreas;
	}

	TerrManagerState TerrManager::CopyState() const
	{
		return m_state;
	}

	void TerrManager::PasteState(const TerrManagerState& state)
	{
		m_state = state;
	}
}
