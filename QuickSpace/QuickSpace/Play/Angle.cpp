﻿#include "stdafx.h"
#include "Angle.h"

#include "QuickSpace/Util/Range.h"

namespace QuickSpace::Play
{
	EAngle Angle::Value() const
	{
		return m_value;
	}

	EAngle Angle::Reverse() const
	{
		switch (m_value)
		{
		case EAngle::Up:
			return EAngle::Down;
		case EAngle::Right:
			return EAngle::Left;
		case EAngle::Down:
			return EAngle::Up;
		case EAngle::Left:
			return EAngle::Right;
		default:
			assert(false);
		}
		return {};
	}

	Float2 Angle::ToFloat2() const
	{
		switch (m_value)
		{
		case EAngle::Up:
			return Float2::Up();
		case EAngle::Right:
			return Float2::Right();
		case EAngle::Down:
			return Float2::Down();
		case EAngle::Left:
			return Float2::Left();
		default:
			assert(false);
		}
		return Float2::Zero();
	}

	Angle Angle::ConvertFrom(const Vec2& pos)
	{
		// atan2の定義域は -pi ~ pi

		double theta = std::atan2(pos.y, pos.x);

		if (Util::RangeDouble(-Math::Pi / 4, Math::Pi / 4).IsBetween(theta))
		{
			return Angle(EAngle::Right);
		}
		else if (Util::RangeDouble(-Math::Pi * 3 / 4, -Math::Pi / 4).IsBetween(theta))
		{
			return Angle(EAngle::Up);
		}
		else if (Util::RangeDouble(Math::Pi / 4, Math::Pi * 3 / 4).IsBetween(theta))
		{
			return Angle(EAngle::Down);
		}
		else
		{
			return Angle(EAngle::Left);
		}
	}
}