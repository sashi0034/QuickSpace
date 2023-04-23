#pragma once
#include "ActorBase.h"
#include "CoroTask.h"

namespace QuickSpace
{
	class CoroActor : public ActorBase
	{
	public:
		explicit CoroActor(CoroTaskFunc task);
		explicit CoroActor(CoroTaskCall* task);
		[[nodiscard]] CoroTaskCall& GetTask() const;
		void Update() override;
	private:
		std::unique_ptr<CoroTaskCall> m_task;
	};
}
