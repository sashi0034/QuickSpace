#include "stdafx.h"
#include "EnKnightNormal.h"

#include "QuickSpace/ConstParam.h"
#include "QuickSpace/GameAsset.h"
#include "QuickSpace/Play/PlayManager.h"
#include "QuickSpace/Play/TerrManager.h"
#include "QuickSpace/Util/Utils.h"

namespace QuickSpace::Play::En
{
	void EnKnightNormal::Init()
	{
		EnemyBase::Init();
	}

	void EnKnightNormal::Update()
	{
		EnemyBase::Update();

		m_animValue += Scene::DeltaTime();

		movePos();

		drawImage();
	}

	void EnKnightNormal::movePos()
	{
		m_moveAngleSpeed = m_moveSpeed / 20.0f;
		m_moveAngle += m_moveAngleSpeed * Scene::DeltaTime();
		const float moveAngleRad = m_moveAngle / 180 * Math::Pi;
		const float dx = Math::Cos(moveAngleRad) * Scene::DeltaTime() * m_moveSpeed;
		const float dy = Math::Sin(moveAngleRad) * Scene::DeltaTime() * m_moveSpeed;

		auto newPos = Pos() + Float2{dx, dy};
		if (PlayManager::Instance().Territory().Frontier().JudgePointInside(newPos.asPoint()) == false)
		{
			newPos = Pos().asPoint();
			m_moveAngle += 90;
		}

		SetPos(newPos);
	}

	void EnKnightNormal::drawImage() const
	{
		const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

		constexpr int cellSize = 32;
		(void)GameAsset::Instance()
			  .temple_knight_side_32x32(cellSize * Util::AnimFrameIndexF(m_animValue, 4, 250), 0, cellSize, cellSize)
			  .scaled(ConstParam::PixelartScale)
			  .drawAt(Pos() + Vec2::Up(ConstParam::PixelartScale * cellSize / 2));
	}
}
