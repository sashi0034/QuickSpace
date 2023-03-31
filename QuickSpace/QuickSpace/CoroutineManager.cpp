#include "../stdafx.h"
#include "CoroutineManager.h"

namespace QuickSpace
{
	CoroutineElement::CoroutineElement(CoroTaskCall* task) :
		m_task(std::unique_ptr<CoroTaskCall>(task))
	{}

	CoroTaskCall& CoroutineElement::GetTask()
	{
		return *m_task;
	}

	void CoroutineElement::Kill()
	{
		m_isDead = true;
	}

	bool CoroutineElement::IsAlive() const
	{
		return !m_isDead;
	}

	std::shared_ptr<CoroutineElement> CoroutineManager::Start(std::function<void(CoroTaskYield&)> task)
	{
		auto product = std::make_shared<CoroutineElement>(new CoroTaskCall(task));
		m_coroList.push_back(product);
		return product;
	}

	void CoroutineManager::Update()
	{
		for (int i = m_coroList.size() - 1; i >= 0; i--)
		{
			auto&& coro = m_coroList[i];
			if (coro->IsAlive() == false)
			{
				m_coroList.erase(m_coroList.begin() + i);
				continue;
			}

			auto&& task = coro->GetTask();
			(task)();
			if (task)
				{}
			else
				coro->Kill();
		}
	}
}
