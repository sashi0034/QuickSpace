#include "stdafx.h"
#include "EnKnightNormal.h"

#include "QuickSpace/ConstParam.h"
#include "QuickSpace/GameAsset.h"

namespace QuickSpace::Play::En
{
	void EnKnightNormal::Init()
	{
		EnemyBase::Init();
	}

	void EnKnightNormal::Update()
	{
		EnemyBase::Update();
		(void)GameAsset::Instance()
			.temple_knight_side_32x32(0, 0, 32, 32)
			.scaled(ConstParam::PixelartScale)
			.drawAt(Pos());
	}
}
