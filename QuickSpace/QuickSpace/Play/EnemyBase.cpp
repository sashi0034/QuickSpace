#include "stdafx.h"
#include "EnemyBase.h"

namespace QuickSpace::Play
{
	EnemyBase::EnemyBase(Point initialPos) :
		m_pos(initialPos)
	{}

	void EnemyBase::Init()
	{}

	EnemyBase& EnemyBase::SetPos(const Float2& pos)
	{
		m_pos = pos;
		return *this;
	}

	EnemyBase& EnemyBase::SetPos(const Point& pos)
	{
		m_pos = pos;
		return *this;
	}

	Float2 EnemyBase::Pos() const
	{
		return m_pos;
	}
}
