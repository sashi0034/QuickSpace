#include "stdafx.h"
#include "Player.h"

#include "PlayManager.h"
#include "SepEdgeSet.h"
#include "TerrManager.h"
#include "QuickSpace/ConstParam.h"
#include "QuickSpace/GameAsset.h"
#include "QuickSpace/GameInput.h"
#include "QuickSpace/GameRoot.h"
#include "QuickSpace/Util/Utils.h"

namespace QuickSpace::Play
{
	Player::Player()
	{}

	void Player::Init()
	{
		auto&& territory = PlayManager::Instance().Territory();
		m_edgeTarget = territory.Edges()[0];
		m_edgeCursor = m_edgeTarget->GetStart()->xy();
	}

	void Player::Update()
	{
		m_animValue += Scene::DeltaTime();

		const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

		const auto position = m_edgeCursor;
		constexpr int cellSize = 32;
		const int cellIndex = Util::AnimFrameIndex(m_animValue, 4, 250);
		(void)GameAsset::Instance().phine_32x32(cellIndex * cellSize, 0, cellSize, cellSize)
			.scaled(ConstParam::PixelartScale)
			.drawAt(position - Vec2{0, ConstParam::PixelartScale * cellSize / 2});


		switch (m_state)
		{
		case EPlayerState::Moving:
			moveOnEdge();
			break;
		case EPlayerState::Drawing:
			moveWithDraw();
			break;
		default:
			assert(false);
		}

		ActorBase::Update();
	}

	Point Player::roundEdgeCursor() const
	{
		return m_edgeCursor.asPoint();
	}

	float Player::getSpeed() const
	{
		return Scene::DeltaTime() * 1000.0f * 0.5f;
	}

	void Player::changeEdgeTargetAutoAfterMoved()
	{
		if (m_edgeCursor.asPoint() == *m_edgeTarget->GetStart())
		{
			for (auto&& edge : m_edgeTarget->Neighbors())
			{
				if (*edge.OverlappedVertex != *m_edgeTarget->GetStart()) continue;
				if (auto&& neighborRef = edge.NeighborRef.lock()) m_edgeTarget = neighborRef;
			}
		}
		else if (m_edgeCursor.asPoint() == *m_edgeTarget->GetEnd())
		{
			for (auto&& edge : m_edgeTarget->Neighbors())
			{
				if (*edge.OverlappedVertex != *m_edgeTarget->GetEnd()) continue;
				if (auto&& neighborRef = edge.NeighborRef.lock()) m_edgeTarget = neighborRef;
			}
		}
	}

	void Player::moveOnEdge()
	{
		const float speed = getSpeed();
		auto edgeDirection = m_edgeTarget->Direction();

		// 辺と同じ方向
		if (inputAngle(edgeDirection).pressed())
		{
			m_edgeTarget->MoveOnEdge(&m_edgeCursor, edgeDirection, speed);
			changeEdgeTargetAutoAfterMoved();
		}
		else if (inputAngle(Angle(edgeDirection).Reverse()).pressed())
		{
			m_edgeTarget->MoveOnEdge(&m_edgeCursor, Angle(edgeDirection).Reverse(), speed);
			changeEdgeTargetAutoAfterMoved();
		}
		// 直角向きの入力
		else if (inputAngle(Angle(edgeDirection).Clockwise()).pressed())
			checkMoveIntersect(Angle(edgeDirection).Clockwise(), speed, m_edgeTarget->IsHorizontal());
		// 逆直角向きの入力
		else if (inputAngle(Angle(edgeDirection).Counterclockwise()).pressed())
			checkMoveIntersect(Angle(edgeDirection).Counterclockwise(), speed, m_edgeTarget->IsHorizontal());
	}

	void Player::checkFinishDrawing()
	{
		const auto intersected = PlayManager::Instance().Territory().Frontier().IntersectWith(SepEdge(m_edgeTarget));
		if (intersected.has_value() == false) return;
		auto intersectedPoint = intersected.value().CalcIntersected(SepEdge(m_edgeTarget));
		if (intersectedPoint == *m_edgeTarget->GetStart()) return;

		// 線を引く処理終了

		finishDrawing(intersectedPoint);
	}

	void Player::finishDrawing(Point intersectedPoint)
	{
		confirmDrawingEdge();
		m_edgeCursor = intersectedPoint;
		*m_edgeTarget->GetEnd() = m_edgeCursor.asPoint();

		for (auto&& edge : PlayManager::Instance().Territory().Edges())
		{
			if (m_edgeTarget->IsHorizontal() == edge->IsHorizontal()) continue;
			if (edge->IsOverlappedVertex(m_edgeTarget->GetEnd()) == false) continue;

			// 頂点が重なっている辺を接続
			TerrEdge::ConnectEdges(edge, m_edgeTarget);
		}

		m_state = EPlayerState::Moving;

		// グラフを分割
		auto tempFrontier = SepEdgeSet(PlayManager::Instance().Territory().Frontier().Edges());
		tempFrontier.TryDivideEdge(m_drawnEdges.front().GetStart());
		tempFrontier.TryDivideEdge(m_drawnEdges.back().GetEnd());
		auto dividedFrontier = tempFrontier.CalcRootAsPureCircuit(*m_drawnEdges.front().GetStart(), *m_drawnEdges.back().GetEnd());

		PlayManager::Instance().Territory().ResetFrontier(SepFace(dividedFrontier.LongRoot.Edges().append(m_drawnEdges)));

		m_drawnEdges.clear();
	}

	void Player::confirmDrawingEdge()
	{
		m_drawnEdges.push_back(SepEdge(m_edgeTarget));
	}

	void Player::extendDrawingEdge(const EAngle direction)
	{
		// 描画中に線を引き伸ばす
		m_edgeCursor += Angle(direction).ToFloat2() * getSpeed();
		*m_edgeTarget->GetEnd() = m_edgeCursor.asPoint();

		const auto cursorExtended =
			std::make_shared<Point>(m_edgeCursor.asPoint() + Angle(direction).ToPoint() * ConstParam::LineMargin);
		const auto cursorExtendedEdge = SepEdge(m_edgeTarget->GetStart(), cursorExtended);
		// 他の描画中の辺と交わらないようにする
		for (auto&& drawnEdge : m_drawnEdges)
		{
			if (drawnEdge.GetEnd() == m_edgeTarget->GetStart()) continue;
			// 同じ向きのやつとも重ならないようにするためちょっとだけ間隔を開けておく
			// share_ptrここで使うの重いから別の方法がいいかも...
			auto drawnEdgeExtended = SepEdge(
				std::make_shared<Point>(*drawnEdge.GetStart() - drawnEdge.GetDirection().ToPoint() * (ConstParam::LineMargin - 1)),
				std::make_shared<Point>(*drawnEdge.GetEnd() + drawnEdge.GetDirection().ToPoint() * (ConstParam::LineMargin - 1)));
			if (drawnEdgeExtended.IsIntersectWith(SepEdge(cursorExtendedEdge)) == false) continue;

			auto intersectedPoint = drawnEdgeExtended.CalcIntersected(SepEdge(m_edgeTarget));
			// 交わったので修正
			*cursorExtended = intersectedPoint;
			m_edgeCursor = intersectedPoint - Angle(direction).ToPoint() * ConstParam::LineMargin;
			*m_edgeTarget->GetEnd() = m_edgeCursor.asPoint();
		}
	}

	void Player::moveWithDraw()
	{
		const auto direction = m_edgeTarget->Direction();

		if (inputAngle(direction).pressed())
		{
			extendDrawingEdge(direction);

			// 描画終了かチェック
			checkFinishDrawing();
		}
		// 一定距離以上ドローしたら線の方向切り替えチェック
		else if (m_edgeTarget->GetLength() > ConstParam::LineMargin)
		{
			if (inputAngle(Angle(direction).Clockwise()).pressed())
				rotateDrawingDirection(Angle(direction).Clockwise());
			else if (inputAngle(Angle(direction).Counterclockwise()).pressed())
				rotateDrawingDirection(Angle(direction).Counterclockwise());
		}
	}

	void Player::rotateDrawingDirection(EAngle angle)
	{
		if (canRotateDrawingDirection(angle) == false) return;

		confirmDrawingEdge();
		continueDrawing(angle, m_edgeTarget->GetEnd());
	}


	bool Player::canRotateDrawingDirection(EAngle angle)
	{
		// 描画中の線の向きを変更できるかチェック

		const auto cursorExtendedEdge = std::make_shared<SepEdge>(
			std::make_shared<Point>(m_edgeCursor.asPoint()),
			std::make_shared<Point>(m_edgeCursor.asPoint() + Angle(angle).ToPoint() * (ConstParam::LineMargin + 1)));

		// 他の描画中の辺と交わらないようにする
		for (auto&& drawnEdge : m_drawnEdges)
		{
			// 同じ向きのやつとも重ならないようにするためちょっとだけ間隔を開けておく
			// share_ptrここで使うの重いから別の方法がいいかも...
			auto drawnEdgeExtended = SepEdge(
				std::make_shared<Point>(*drawnEdge.GetStart() - drawnEdge.GetDirection().ToPoint() * (ConstParam::LineMargin - 1)),
				std::make_shared<Point>(*drawnEdge.GetEnd() + drawnEdge.GetDirection().ToPoint() * (ConstParam::LineMargin - 1)));
			if (drawnEdgeExtended.IsIntersectWith(*cursorExtendedEdge) == false) continue;

			// 交わったのでキャンセル
			return false;
		}
		return true;
	}

	float getNeighborDelta(const TerrEdgeNeighbor neighbor, Point cursor, bool isHorizontal)
	{
		return isHorizontal
			? neighbor.OverlappedVertex->x - cursor.x
			: neighbor.OverlappedVertex->y - cursor.y;
	}

	bool Player::checkStartDrawing(EAngle angle)
	{
		if (GameInput::Instance().Ok().pressed() == false) return false;

		// TODO: 線を引いてもいい場所かチェックする処理
		// 線を引けるので開始
		startDrawing(angle);
		return true;
	}

	// 移動
	void Player::checkMoveIntersect(EAngle angle, float speed, bool isHorizontal)
	{
		// 最も近い隣接辺を対象にする
		const auto neighbor0 = m_edgeTarget->GetNearestNeighbor(m_edgeCursor, angle);
		if (neighbor0.has_value() == false)
		{
			checkStartDrawing(angle);
			return;
		}

		const auto neighbor = neighbor0.value();
		const float neighborDelta = getNeighborDelta(neighbor, roundEdgeCursor(), isHorizontal);
		if (Math::Abs(neighborDelta) > 1)
		{
			if (checkStartDrawing(angle)) return;

			// 方向が違う隣接辺に遠いので近づく
			if (neighborDelta < 0) m_edgeTarget->MoveOnEdge(&m_edgeCursor, isHorizontal ? EAngle::Left : EAngle::Up, speed);
			else m_edgeTarget->MoveOnEdge(&m_edgeCursor, isHorizontal ? EAngle::Right : EAngle::Down, speed);

			// 点を通過してしまったときはその点に強制的に戻す
			const float newNeighborDelta = getNeighborDelta(neighbor, roundEdgeCursor(), isHorizontal);
			if (Math::Sign(newNeighborDelta) != Math::Sign(neighborDelta))
				m_edgeCursor = neighbor.OverlappedVertex->xy();
		}
		else if (auto&& neighborRef = neighbor.NeighborRef.lock())
		{
			// 辺切り替え
			m_edgeTarget = neighborRef;
		}
	}

	InputGroup& Player::inputAngle(EAngle angle)
	{
		switch (angle)
		{
		case EAngle::Up: return GameInput::Instance().Up();
		case EAngle::Right: return GameInput::Instance().Right();
		case EAngle::Down: return GameInput::Instance().Down();
		case EAngle::Left: return GameInput::Instance().Left();
		default:
			assert(false);
			return GameInput::Instance().Up();
		}
	}

	void Player::continueDrawing(EAngle angle, const TerrVertexRef& oldEnd)
	{
		auto nearPoint = roundEdgeCursor() + Angle(angle).ToPoint();
		const auto newEdge = std::make_shared<TerrEdge>(
			oldEnd,
			std::make_shared<Point>(nearPoint.x, nearPoint.y));
		TerrEdge::ConnectEdges(newEdge, m_edgeTarget);
		newEdge->SetFixed(false);
		PlayManager::Instance().Territory().Edges().push_back(newEdge);
		m_edgeTarget = newEdge;
	}

	void Player::startDrawing(EAngle angle)
	{
		m_state = EPlayerState::Drawing;
		m_drawnEdges.clear();
		continueDrawing(angle, std::make_shared<Point>(roundEdgeCursor()));
	}


	float Player::OrderPriority()
	{
		return 100;
	}

	TerrEdgeRef& Player::GetEdgeTarget()
	{
		return m_edgeTarget;
	}

	// Optional<EAngle> Player::getInputAngle() const
	// {
	// 	if (GameInput::Instance().Up().pressed()) return EAngle::Up;
	// 	if (GameInput::Instance().Down().pressed()) return EAngle::Down;
	//
	// 	if (GameInput::Instance().Left().pressed()) return EAngle::Left;
	// 	if (GameInput::Instance().Right().pressed()) return EAngle::Right;
	// }
}
