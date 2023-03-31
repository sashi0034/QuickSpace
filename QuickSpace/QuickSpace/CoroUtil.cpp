﻿#include "../stdafx.h"
#include "CoroUtil.h"

namespace QuickSpace
{
	CoroTask CoroUtil::WaitForTime(CoroTaskYield& yield, double seconds)
	{
		double totalTime = 0;
		while (totalTime < seconds)
		{
			totalTime += Scene::DeltaTime();
			yield();
		}
	}

	CoroTask CoroUtil::WaitForTrue(CoroTaskYield& yield, const std::function<bool()>& discriminant)
	{
		while (!discriminant())
		{
			yield();
		}
	}

	CoroTask CoroUtil::WaitForFalse(CoroTaskYield& yield, const std::function<bool()>& discriminant)
	{
		while (discriminant())
		{
			yield();
		}
	}

	CoroTask CoroUtil::WaitForCoro(CoroTaskYield& yield, std::shared_ptr<CoroutineElement> coro)
	{
		WaitForFalse(yield, [coro](){return coro->IsAlive(); });
	}
}