#include "../stdafx.h"
#include "ActorBase.h"

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

	void ActorBase::SetParent(std::weak_ptr<ActorBase> parent)
	{
		m_parent = parent;
	}

	std::weak_ptr<ActorBase>& ActorBase::GetParent()
	{
		return m_parent;
	}

	ActorBaseInternal& ActorBase::Internal()
	{
		return m_internal;
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
