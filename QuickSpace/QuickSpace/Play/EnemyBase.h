#pragma once
#include "QuickSpace/ActorBase.h"

namespace QuickSpace::Play
{
	class EnemyBase : public ActorBase
	{
	public:
		EnemyBase() = default;
		explicit EnemyBase(Point initialPos);
		virtual void Init();

		EnemyBase& SetPos(const Float2& pos);
		EnemyBase& SetPos(const Point& pos);
		Float2 Pos() const;
	private:
		Float2 m_pos{};
	};
}
