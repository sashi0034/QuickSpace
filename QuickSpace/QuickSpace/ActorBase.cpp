#include "../stdafx.h"
#include "ActorBase.h"
#include "ActorManager.h"

namespace QuickSpace
{
	void ActorBase::Kill()
	{
		m_isAlive = false;
	}

	bool ActorBase::IsDead() const
	{
		return !m_isAlive;
	}


	bool ActorBase::HasChildren() const
	{
		return m_children.get() != nullptr;
	}

	ActorManager& ActorBase::AsParent()
	{
		if (m_children.get() == nullptr) m_children.reset(new ActorManager());
		return *m_children;
	}

	void ActorBase::Update()
	{
		if (m_children.get() == nullptr) return;
		m_children->Update();
	}

	float ActorBase::OrderPriority()
	{
		return 0;
	}

	void ActorBase::SetActive(bool isActive)
	{
		m_isActive = isActive;
	}

	bool ActorBase::IsActive() const
	{
		return m_isActive;
	}
}
