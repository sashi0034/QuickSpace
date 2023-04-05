#include "stdafx.h"
#include "Player.h"

#include "PlayManager.h"
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

	void Player::moveOnEdge()
	{
		const float speed = getSpeed();
		auto edgeDirection = m_edgeTarget->Direction();

		// 辺と同じ方向
		if (inputAngle(edgeDirection).pressed())
			m_edgeTarget->MoveOnEdge(&m_edgeCursor, edgeDirection, speed);
		else if (inputAngle(Angle(edgeDirection).Reverse()).pressed())
			m_edgeTarget->MoveOnEdge(&m_edgeCursor, Angle(edgeDirection).Reverse(), speed);
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
	}

	void Player::moveWithDraw()
	{
		const auto direction = m_edgeTarget->Direction();
		const float speed = getSpeed();

		if (inputAngle(direction).pressed())
		{
			// 描画中に線を引き伸ばす
			m_edgeCursor += Angle(direction).ToFloat2() * speed;
			*m_edgeTarget->GetEnd() = m_edgeCursor.asPoint();

			// 描画終了かチェック
			checkFinishDrawing();
		}
		// 線の方向切り替え
		else if (inputAngle(Angle(direction).Clockwise()).pressed())
		{
			startDrawing(Angle(direction).Clockwise());
		}
		else if (inputAngle(Angle(direction).Counterclockwise()).pressed())
		{
			startDrawing(Angle(direction).Counterclockwise());
		}

	}

	float getNeighborDelta(const TerrEdgeNeighbor neighbor, Point cursor, bool isHorizontal)
	{
		return isHorizontal
			? neighbor.OverlappedVertex->x - cursor.x
			: neighbor.OverlappedVertex->y - cursor.y;
	}

	// 移動
	void Player::checkMoveIntersect(EAngle angle, float speed, bool isHorizontal)
	{
		// 最も近い隣接辺を対象にする
		const auto neighbor0 = m_edgeTarget->GetNearestNeighbor(m_edgeCursor, angle);
		if (neighbor0.has_value() == false) return;

		const auto neighbor = neighbor0.value();
		const float neighborDelta = getNeighborDelta(neighbor, roundEdgeCursor(), isHorizontal);
		if (Math::Abs(neighborDelta) > 1)
		{
			if (GameInput::Instance().Ok().pressed())
			{
				// TODO: 線を引いてもいい場所かチェックする処理
				// 線を引けるので開始
				startDrawing(angle);
				return;
			}

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

	void Player::startDrawing(EAngle angle)
	{
		m_state = EPlayerState::Drawing;
		auto cursorPoint = roundEdgeCursor();
		auto nearPoint = roundEdgeCursor() + Angle(angle).ToPoint();
		const auto newEdge = std::make_shared<TerrEdge>(
			std::make_shared<Point>(cursorPoint.x, cursorPoint.y),
			std::make_shared<Point>(nearPoint.x, nearPoint.y));
		TerrEdge::ConnectEdges(newEdge, m_edgeTarget);
		newEdge->SetFixed(false);
		PlayManager::Instance().Territory().Edges().push_back(newEdge);
		m_edgeTarget = newEdge;
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
