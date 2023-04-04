#include "stdafx.h"
#include "Player.h"

#include "PlayManager.h"
#include "TerrManager.h"
#include "QuickSpace/ConstParam.h"
#include "QuickSpace/GameAsset.h"
#include "QuickSpace/GameInput.h"
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


		moveOnEdge();

		ActorBase::Update();
	}

	void Player::moveOnEdge()
	{
		const float speed = Scene::DeltaTime() * 1000.0f * 0.5f;
		if (m_edgeTarget->IsHorizontal())
		{
			if (GameInput::Instance().Left().pressed())
				m_edgeTarget->MoveOnEdge(&m_edgeCursor, EAngle::Left, speed);
			else if (GameInput::Instance().Right().pressed())
				m_edgeTarget->MoveOnEdge(&m_edgeCursor, EAngle::Right, speed);
			else if (GameInput::Instance().Up().pressed())
				checkMoveToNeighbor(EAngle::Up, speed, true);
			else if (GameInput::Instance().Down().pressed())
				checkMoveToNeighbor(EAngle::Down, speed, true);
		}
		else
		{
			if (GameInput::Instance().Up().pressed())
				m_edgeTarget->MoveOnEdge(&m_edgeCursor, EAngle::Up, speed);
			else if (GameInput::Instance().Down().pressed())
				m_edgeTarget->MoveOnEdge(&m_edgeCursor, EAngle::Down, speed);
			else if (GameInput::Instance().Left().pressed())
				checkMoveToNeighbor(EAngle::Left, speed, false);
			else if (GameInput::Instance().Right().pressed())
				checkMoveToNeighbor(EAngle::Right, speed, false);
		}
	}

	// 方向が違う隣接頂点に移動
	void Player::checkMoveToNeighbor(EAngle angle, float speed, bool isHorizontal)
	{
		const auto neighbor0 = m_edgeTarget->GetNearestNeighbor(m_edgeCursor, angle);
		if (neighbor0.has_value() == false) return;

		const auto neighbor = neighbor0.value();
		const float neighborDelta = isHorizontal
			? neighbor.OverlappedVertex->x - m_edgeCursor.x
			: neighbor.OverlappedVertex->y - m_edgeCursor.y;
		if (Math::Abs(neighborDelta) > speed)
		{
			// 切り替えられる辺に遠いので近づく
			if (neighborDelta < 0) m_edgeTarget->MoveOnEdge(&m_edgeCursor, isHorizontal ? EAngle::Left : EAngle::Up, speed);
			else m_edgeTarget->MoveOnEdge(&m_edgeCursor, isHorizontal ? EAngle::Right : EAngle::Down, speed);
		}
		else if (auto&& neighborRef = neighbor.NeighborRef.lock())
		{
			// 辺切り替え
			m_edgeTarget = neighborRef;
		}
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
