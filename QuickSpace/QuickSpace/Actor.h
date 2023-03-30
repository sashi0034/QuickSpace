#pragma once

namespace QuickSpace
{
	class IActor
	{
	public:
		virtual ~IActor() = default;
		virtual void Update() = 0;
	};
}
