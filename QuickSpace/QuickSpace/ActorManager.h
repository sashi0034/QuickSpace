#pragma once
#include "Actor.h"

namespace QuickSpace
{
	class ActorManager
	{
	public:
		void Update();
		void Clear();
		void Birth(IActor* actor);
		template <typename T> std::shared_ptr<T> BirthAs(T* actor)
		{
			auto product = std::shared_ptr<T>(actor);
			m_actorList.push_back(product);
			return product;
		}
	private:
		std::vector<std::shared_ptr<IActor>> m_actorList{};
	};
}
