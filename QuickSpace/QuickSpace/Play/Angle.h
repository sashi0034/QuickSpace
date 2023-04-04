#pragma once

namespace QuickSpace::Play
{
	enum class EAngle
	{
		Up,
		Right,
		Down,
		Left,
	};

	class Angle
	{
	public:
		explicit Angle(EAngle value) : m_value(value){}
		[[nodiscard]] EAngle Value() const;
		EAngle Reverse() const;
		EAngle Clockwise() const;
		EAngle Counterclockwise() const;
		Point ToPoint() const;
		Float2 ToFloat2() const;

		static Angle ConvertFrom(const Vec2& vec);
	private:
		EAngle m_value;
	};
}
