#pragma once

namespace QuickSpace
{
	class ActorManager;

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

		bool HasChildren() const;
		[[nodiscard]] ActorManager& AsParent();
	private:
		bool m_isActive = true;
		bool m_isAlive = true;
		std::unique_ptr<ActorManager> m_children;
	};
}
