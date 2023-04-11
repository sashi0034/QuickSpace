#include "stdafx.h"
#include "Player.h"

#include "PlayerAction.h"
#include "PlayManager.h"
#include "SepEdgeSet.h"
#include "TerrManager.h"
#include "QuickSpace/ConstParam.h"
#include "QuickSpace/GameInput.h"

namespace QuickSpace::Play
{
	Player::Player() = default;

	void Player::Init()
	{
		Restart();
	}

	void Player::Restart()
	{
		auto&& territory = PlayManager::Instance().Territory();
		m_edgeTarget = territory.Edges()[0];
		m_edgeCursor = m_edgeTarget->GetStart().xy();
		m_state = EPlayerState::Moving;
	}

	void Player::Update()
	{
		m_animValue += Scene::DeltaTime();

		m_edgeCursorBefore = m_edgeCursor;

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

		if (m_edgeCursor != m_edgeCursorBefore) m_angle = Angle::ConvertFrom(m_edgeCursor - m_edgeCursorBefore);
		PlayerAction::DrawPlayer(*this);

		ActorBase::Update();
	}

	Point Player::roundEdgeCursor() const
	{
		return m_edgeCursor.asPoint();
	}

	float Player::getSpeed() const
	{
		return Scene::DeltaTime() * 1000.0f * m_speedBase;
	}

	void Player::changeEdgeTargetAutoAfterMoved()
	{
		if (m_edgeCursor.asPoint() == m_edgeTarget->GetStart())
		{
			for (auto&& edge : m_edgeTarget->Neighbors())
			{
				if (edge.OverlappedVertex != m_edgeTarget->GetStart()) continue;
				if (auto&& neighborRef = edge.NeighborRef.lock()) m_edgeTarget = neighborRef;
			}
		}
		else if (m_edgeCursor.asPoint() == m_edgeTarget->GetEnd())
		{
			for (auto&& edge : m_edgeTarget->Neighbors())
			{
				if (edge.OverlappedVertex != m_edgeTarget->GetEnd()) continue;
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
		for (auto&& checking : PlayManager::Instance().Territory().Frontier().Edges())
		{
			if (checking.IsIntersectWith(SepEdge(m_edgeTarget)) == false) continue;
			const auto intersectedPoint = checking.CalcIntersected(SepEdge(m_edgeTarget));
			if (intersectedPoint == m_edgeTarget->GetStart()) continue;;

			// 線を引く処理終了

			finishDrawing(intersectedPoint);
			break;
		}
	}

	void Player::finishDrawing(Point intersectedPoint)
	{
		m_edgeCursor = intersectedPoint;
		m_edgeTarget->ChangeEnd(m_edgeCursor.asPoint());
		m_drawnEdges.push_back(SepEdge(m_edgeTarget));

		for (auto&& edge : PlayManager::Instance().Territory().Edges())
		{
			// 固定
			edge->SetFixed(true);
		}
		for (auto&& edge : PlayManager::Instance().Territory().Edges())
		{
			if (m_edgeTarget->IsHorizontal() == edge->IsHorizontal()) continue;
			if (edge->IsOverlappedVertex(m_edgeTarget->GetEnd()) == false) continue;

			// 頂点が重なっている辺を接続
			TerrEdge::ConnectEdges(edge, m_edgeTarget);
			break;
		}

		m_state = EPlayerState::Moving;

		// グラフを分割
		auto tempFrontier = SepEdgeSet(PlayManager::Instance().Territory().Frontier().Edges());
		tempFrontier.TryDivideEdge(m_drawnEdges.front().GetStart());
		tempFrontier.TryDivideEdge(m_drawnEdges.back().GetEnd());
		auto dividedFrontier = tempFrontier.CalcRouteAsPureCircuit(m_drawnEdges.front().GetStart(), m_drawnEdges.back().GetEnd());

		PlayManager::Instance().Territory().ResetFrontier(
			SepFace(dividedFrontier.LongRoot.Edges().append(m_drawnEdges)));
		PlayManager::Instance().Territory().AddOccupiedArea(
			SepEdgeSet(dividedFrontier.ShortRoot.Edges().append(m_drawnEdges)));

		m_drawnEdges.clear();
	}

	void Player::extendDrawingEdge(const EAngle direction)
	{
		// 描画中に線を引き伸ばす
		m_edgeCursor += Angle(direction).ToFloat2() * getSpeed();
		m_edgeTarget->ChangeEnd(m_edgeCursor.asPoint());

		const auto cursorExtended = Point(m_edgeCursor.asPoint() + Angle(direction).ToPoint() * ConstParam::LineMargin);
		auto cursorExtendedEdge = SepEdge(m_edgeTarget->GetStart(), cursorExtended);
		// 他の描画中の辺と交わらないようにする
		for (auto&& drawnEdge : m_drawnEdges)
		{
			if (drawnEdge.GetEnd() == m_edgeTarget->GetStart()) continue;
			// 同じ向きのやつとも重ならないようにするためちょっとだけ間隔を開けておく
			auto&& drawnEdgeExtended = SepEdge(drawnEdge).ExtendBothTips(ConstParam::LineMargin - 1);
			if (drawnEdgeExtended.IsIntersectWith(SepEdge(cursorExtendedEdge)) == false) continue;

			auto intersectedPoint = drawnEdgeExtended.CalcIntersected(cursorExtendedEdge);
			// 交わったので修正
			cursorExtendedEdge.ChangeEnd(intersectedPoint);
			m_edgeCursor = intersectedPoint - Angle(direction).ToPoint() * ConstParam::LineMargin;
			m_edgeTarget->ChangeEnd(m_edgeCursor.asPoint());
		}

		checkStickDrawingEdgeToFrontier(direction, cursorExtendedEdge);
	}

	void Player::checkStickDrawingEdgeToFrontier(const EAngle direction, SepEdge cursorExtendedEdge)
	{
		for (auto&& frontierEdge : PlayManager::Instance().Territory().Frontier().Edges())
		{
			if (frontierEdge.IsIntersectWith(cursorExtendedEdge) == false) continue;
			const auto intersectedPoint = frontierEdge.CalcIntersected(cursorExtendedEdge);
			if (intersectedPoint == m_edgeTarget->GetStart()) continue;

			// フロンティアが近くにあったら自動的に吸着
			cursorExtendedEdge.ChangeEnd(intersectedPoint);
			m_edgeCursor = intersectedPoint;
			m_edgeTarget->ChangeEnd(intersectedPoint);

			// 終了
			return;
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

		m_drawnEdges.push_back(SepEdge(m_edgeTarget));
		continueDrawing(angle, m_edgeTarget->GetEnd());
	}


	bool Player::canRotateDrawingDirection(EAngle angle)
	{
		// 描画中の線の向きを変更できるかチェック

		const auto cursorExtendedEdge = std::make_shared<SepEdge>(
				m_edgeCursor.asPoint(),
			m_edgeCursor.asPoint() + Angle(angle).ToPoint() * (ConstParam::LineMargin * 2));

		// 他の描画中の辺と交わらないようにする
		for (auto&& drawnEdge : m_drawnEdges)
		{
			// 同じ向きのやつとも重ならないようにするためちょっとだけ間隔を開けておく
			// share_ptrここで使うの重いから別の方法がいいかも...
			auto drawnEdgeExtended = SepEdge(
				drawnEdge.GetStart() - drawnEdge.GetDirection().ToPoint() * (ConstParam::LineMargin - 1),
				drawnEdge.GetEnd() + drawnEdge.GetDirection().ToPoint() * (ConstParam::LineMargin - 1));
			if (drawnEdgeExtended.IsIntersectWith(*cursorExtendedEdge) == false) continue;

			// 交わったのでキャンセル
			return false;
		}
		return true;
	}

	int getNeighborDelta(const TerrEdgeNeighbor neighbor, Point cursor, bool isHorizontal)
	{
		return isHorizontal
			? neighbor.OverlappedVertex.x - cursor.x
			: neighbor.OverlappedVertex.y - cursor.y;
	}

	bool Player::checkStartDrawing(EAngle angle)
	{
		if (GameInput::Instance().Ok().pressed() == false) return false;
		if (PlayManager::Instance().Territory().Frontier()
			.JudgePointInside(roundEdgeCursor() + Angle(angle).ToPoint()) != true) return false;

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
			if (Math::Abs(neighborDelta) > ConstParam::LineMargin && checkStartDrawing(angle)) return;

			// 方向が違う隣接辺に遠いので近づく
			if (neighborDelta < 0) m_edgeTarget->MoveOnEdge(&m_edgeCursor, isHorizontal ? EAngle::Left : EAngle::Up, speed);
			else m_edgeTarget->MoveOnEdge(&m_edgeCursor, isHorizontal ? EAngle::Right : EAngle::Down, speed);

			// 点を通過してしまったときはその点に強制的に戻す
			const float newNeighborDelta = getNeighborDelta(neighbor, roundEdgeCursor(), isHorizontal);
			if (Math::Sign(newNeighborDelta) != Math::Sign(neighborDelta))
				m_edgeCursor = neighbor.OverlappedVertex.xy();
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

	void Player::continueDrawing(EAngle angle, const TerrVertex& oldEnd)
	{
		const auto nearPoint = roundEdgeCursor() + Angle(angle).ToPoint();
		const auto newEdge = std::make_shared<TerrEdge>(
			oldEnd,
			Point(nearPoint.x, nearPoint.y));
		TerrEdge::ConnectEdges(newEdge, m_edgeTarget);
		newEdge->SetFixed(false);
		PlayManager::Instance().Territory().Edges().push_back(newEdge);
		m_edgeTarget = newEdge;
	}

	void Player::startDrawing(EAngle angle)
	{
		m_state = EPlayerState::Drawing;
		m_drawnEdges.clear();
		continueDrawing(angle, roundEdgeCursor());
	}


	float Player::OrderPriority()
	{
		return 100;
	}

	TerrEdgeRef& Player::GetEdgeTarget()
	{
		return m_edgeTarget;
	}

	void Player::ForceMoveEdgeCursor(Float2 point)
	{
		m_edgeCursor = point;
	}

	Float2 Player::EdgeCursor() const
	{
		return m_edgeCursor;
	}

	bool Player::IsMovingCursorNow() const
	{
		return m_edgeCursor != m_edgeCursorBefore;
	}

	Angle Player::GetAngle() const
	{
		return m_angle;
	}

	float& Player::MutSpeedBase()
	{
		return m_speedBase;
	}
}
