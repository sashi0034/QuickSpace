#pragma once

namespace QuickSpace
{
	using CoroTask = void;
	using CoroTaskCall = boost::coroutines2::coroutine<CoroTask>::pull_type;
	using CoroTaskYield = boost::coroutines2::coroutine<CoroTask>::push_type;

	class CoroutineElement
	{
	public:
		explicit CoroutineElement(CoroTaskCall* task);
		[[nodiscard]] CoroTaskCall& GetTask();
		void Kill();
		bool IsAlive() const;
	private:
		std::unique_ptr<CoroTaskCall> m_task;
		bool m_isDead = false;
	};

	class CoroutineManager
	{
	public:
		std::shared_ptr<CoroutineElement> Start(std::function<void(CoroTaskYield&)> task);
		void Update();
	private:
		std::vector<std::shared_ptr<CoroutineElement>> m_coroList{};
	};
}
