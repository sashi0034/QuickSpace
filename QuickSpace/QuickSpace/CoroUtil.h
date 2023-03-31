#pragma once
#include "CoroutineManager.h"

namespace QuickSpace
{
	namespace CoroUtil
	{
		CoroTask WaitForTime(CoroTaskYield& yield, double seconds);
		CoroTask WaitForTrue(CoroTaskYield& yield, const std::function<bool()>& discriminant);
		CoroTask WaitForFalse(CoroTaskYield& yield, const std::function<bool()>& discriminant);
		CoroTask WaitForCoro(CoroTaskYield& yield, std::shared_ptr<CoroutineElement> coro);
	};
}
