#include "stdafx.h"
#include "Player.h"

#include "PlayManager.h"
#include "TerritoryManager.h"
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
	}

	void Player::Update()
	{
		m_animValue += Scene::DeltaTime();

		const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

		const auto position = m_edgeTarget.Midpoint(m_edgeProceededRate);
		constexpr int cellSize = 32;
		const int cellIndex = Util::AnimFrameIndex(m_animValue, 4, 250);
		(void)GameAsset::Instance().phine_32x32(cellIndex * cellSize, 0, cellSize, cellSize)
			.scaled(ConstParam::PixelartScale)
			.drawAt(position - Vec2{0, ConstParam::PixelartScale * cellSize / 2});


		const float speed = Scene::DeltaTime() * 0.3f;
		if (GameInput::Instance().Up().pressed())
			m_edgeTarget.MoveOnEdge(&m_edgeProceededRate, EAngle::Up, speed);
		else if (GameInput::Instance().Down().pressed())
			m_edgeTarget.MoveOnEdge(&m_edgeProceededRate, EAngle::Down, speed);
		if (GameInput::Instance().Left().pressed())
			m_edgeTarget.MoveOnEdge(&m_edgeProceededRate, EAngle::Left, speed);
		else if (GameInput::Instance().Right().pressed())
			m_edgeTarget.MoveOnEdge(&m_edgeProceededRate, EAngle::Right, speed);

		ActorBase::Update();
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
