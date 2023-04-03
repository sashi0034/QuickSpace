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

		static Angle ConvertFrom(const Vec2& vec);
	private:
		EAngle m_value;
	};
}
