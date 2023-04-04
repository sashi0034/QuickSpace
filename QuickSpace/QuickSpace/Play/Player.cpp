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
		m_edgeTarget = territory.List()[0];
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

	void Player::moveWithDraw()
	{
		const auto direction = m_edgeTarget->Direction();
		const float speed = getSpeed();

		if (inputAngle(direction).pressed())
		{
			m_edgeCursor += Angle(direction).ToFloat2() * speed;
			*m_edgeTarget->GetEnd() = m_edgeCursor.asPoint();
		}
		else if (inputAngle(Angle(direction).Clockwise()).pressed())
		{
			startDrawing(Angle(direction).Clockwise());
		}
		else if (inputAngle(Angle(direction).Counterclockwise()).pressed())
		{
			startDrawing(Angle(direction).Counterclockwise());
		}

	}

	// 移動
	void Player::checkMoveIntersect(EAngle angle, float speed, bool isHorizontal)
	{
		const auto neighbor0 = m_edgeTarget->GetNearestNeighbor(m_edgeCursor, angle);
		if (neighbor0.has_value() == false) return;

		const auto neighbor = neighbor0.value();
		const float neighborDelta = isHorizontal
			? neighbor.OverlappedVertex->x - roundEdgeCursor().x
			: neighbor.OverlappedVertex->y - roundEdgeCursor().y;
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
		PlayManager::Instance().Territory().List().push_back(newEdge);
		m_edgeTarget = newEdge;
	}


	float Player::OrderPriority()
	{
		return 100;
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
