#pragma once

namespace QuickSpace
{
	class ActorBase
	{
	public:
		virtual ~ActorBase() = default;
		virtual void Update(){};
		virtual float OrderPriority();
	};
}
