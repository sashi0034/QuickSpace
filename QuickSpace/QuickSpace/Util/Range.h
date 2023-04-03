#pragma once

namespace QuickSpace::Util
{
	template <typename T>
	class Range
	{
	private:
		const T m_Min;
		const T m_Max;
	public:
		Range(T minInclusive, T maxInclusive)
		: m_Min(minInclusive), m_Max(maxInclusive)
		{
			assert(minInclusive <= maxInclusive);
		}
		T GetDiff() const{
			return m_Max - m_Min;
		}
		bool IsBetween(T targetValue) const
		{
			return m_Min <= targetValue && targetValue <= m_Max;
		}

		T Clamp(T targetValue) const
		{
			return (std::max)(m_Min, (std::min)(targetValue, m_Max));
		}
		T ClampWithAssert(T targetValue) const
		{
			assert(IsBetween(targetValue));
			return Clamp(targetValue);
		}

		T Normalize(T targetValue, const Range<T>&afterRange)
		{
			assert(this->IsBetween(targetValue));
			T normalizedRate = (targetValue - this->m_Min) / (this->m_Max - this->m_Min);
			T result = afterRange.m_Min + (afterRange.m_Max - afterRange.m_Min) * normalizedRate;
			return result;
		}

	};

	using RangeInt = Range<int>;
	using RangeDouble = Range<double>;
	using RangeF = Range<float>;
}
