#pragma once

namespace QuickSpace
{
	struct ActorBaseInternal
	{
		// Managerでこの状態を親に追従させる際などで使用
		bool HasFollowedParent{};
		bool IsActiveInHierarchy{};
	};

	class ActorBase
	{
	public:
		virtual ~ActorBase() = default;
		virtual void Update(){};
		virtual float OrderPriority();

		void SetActive(bool isActive);
		bool IsActive() const;

		void Kill();
		bool IsDead() const;

		void SetParent(std::weak_ptr<ActorBase> parent);
		std::weak_ptr<ActorBase>& GetParent();

		ActorBaseInternal& Internal();
	private:
		bool m_isActive = true;
		bool m_isAlive = true;
		std::weak_ptr<ActorBase> m_parent{};

		ActorBaseInternal m_internal{};
	};
}
