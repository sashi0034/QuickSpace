#pragma once

namespace QuickSpace
{
	using CoroTask = void;
	using CoroTaskCall = boost::coroutines2::coroutine<CoroTask>::pull_type;
	using CoroTaskYield = boost::coroutines2::coroutine<CoroTask>::push_type;

	class CoroElem
	{
	public:
		explicit CoroElem(CoroTaskCall* task);
		[[nodiscard]] CoroTaskCall& GetTask();
		void Kill();
		bool IsAlive() const;
	private:
		std::unique_ptr<CoroTaskCall> m_task;
		bool m_isDead = false;
	};

	using CoroElemShared = std::shared_ptr<CoroElem>;

	class CoroManager
	{
	public:
		std::shared_ptr<CoroElem> Start(std::function<void(CoroTaskYield&)> task);
		void Update();
	private:
		std::vector<CoroElemShared> m_coroList{};
	};
}
