﻿#pragma once
#include "ActorBase.h"
#include "CoroTask.h"

namespace QuickSpace
{
	class ActorBase;
	class CoroActor;

	class ActorManager
	{
	public:
		static ActorManager& Global();
		void Update();
		void Clear();
		void Birth(ActorBase* actor);
		template <typename T> std::shared_ptr<T> BirthAs(T* actor)
		{
			auto product = std::shared_ptr<T>(actor);
			m_actorList.push_back(product);
			return product;
		}
		std::shared_ptr<CoroActor> StartCoro(CoroTaskFunc coro);
		std::vector<std::shared_ptr<ActorBase>>& ActorList();
	private:
		std::vector<std::shared_ptr<ActorBase>> m_actorList{};
		void sortActorList();
	};
}
