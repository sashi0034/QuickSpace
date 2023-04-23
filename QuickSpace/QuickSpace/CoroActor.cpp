#include "../stdafx.h"
#include "CoroActor.h"

#include "GameRoot.h"

namespace QuickSpace
{
	CoroActor::CoroActor(CoroTaskFunc task) :
		CoroActor(new CoroTaskCall(task))
	{}

	CoroActor::CoroActor(CoroTaskCall* task) :
		m_task(std::unique_ptr<CoroTaskCall>(task))
	{}

	CoroTaskCall& CoroActor::GetTask() const
	{
		return *m_task;
	}

	void CoroActor::Update()
	{
		if ((*m_task)())
			{}
		else
			ActorBase::Kill();
	}
}
