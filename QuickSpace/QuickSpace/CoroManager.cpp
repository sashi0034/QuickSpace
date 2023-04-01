#include "../stdafx.h"
#include "CoroManager.h"

#include "GameRoot.h"

namespace QuickSpace
{
	CoroElem::CoroElem(CoroTaskCall* task) :
		m_task(std::unique_ptr<CoroTaskCall>(task))
	{}

	CoroTaskCall& CoroElem::GetTask()
	{
		return *m_task;
	}

	void CoroElem::Kill()
	{
		m_isDead = true;
	}

	bool CoroElem::IsAlive() const
	{
		return !m_isDead;
	}

	std::shared_ptr<CoroElem> CoroManager::Start(std::function<void(CoroTaskYield&)> task)
	{
		auto product = std::make_shared<CoroElem>(new CoroTaskCall(task));
		m_coroList.push_back(product);
		return product;
	}

	void CoroManager::Update()
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

	CoroManager& CoroManager::Global()
	{
		return GameRoot::Global().GetCoroutineManager();
	}
}
