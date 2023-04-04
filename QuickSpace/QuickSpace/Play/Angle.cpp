#include "stdafx.h"
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

	EAngle Angle::Clockwise() const
	{
		switch (m_value)
		{
		case EAngle::Up:
			return EAngle::Right;
		case EAngle::Right:
			return EAngle::Down;
		case EAngle::Down:
			return EAngle::Left;
		case EAngle::Left:
			return EAngle::Up;
		default:
			assert(false);
		}
		return {};
	}

	EAngle Angle::Counterclockwise() const
	{
		switch (m_value)
		{
		case EAngle::Up:
			return EAngle::Left;
		case EAngle::Right:
			return EAngle::Up;
		case EAngle::Down:
			return EAngle::Right;
		case EAngle::Left:
			return EAngle::Down;
		default:
			assert(false);
		}
		return {};
	}

	template <typename T> inline T convertToVector(EAngle angle)
	{
		switch (angle)
		{
		case EAngle::Up:
			return T::Up();
		case EAngle::Right:
			return T::Right();
		case EAngle::Down:
			return T::Down();
		case EAngle::Left:
			return T::Left();
		default:
			assert(false);
		}
		return T::Zero();
	}

	Point Angle::ToPoint() const
	{
		return convertToVector<Point>(m_value);
	}

	Float2 Angle::ToFloat2() const
	{
		return convertToVector<Float2>(m_value);
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
