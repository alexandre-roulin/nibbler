#pragma once

#include <map>
#include <typeindex>
#include "Pool.hpp"
#include <KNU/entities/Entity.hpp>
#include <KNU/component/TransformComponent.hpp>

namespace KNU {

	class ComponentManager {
	private:
		std::map<std::type_index, IPool *> _pools;
	public:
		ComponentManager();

		template<typename ComponentType>
		void addComponent(Entity &e, ComponentType &component);
	};

	ComponentManager::ComponentManager() {
		IPool * pool = new Pool<KNU::TransformComponent>();
		std::type_index e = typeid(KNU::TransformComponent);
		_pools[e] = pool;
	}

	template<typename ComponentType>
	void ComponentManager::addComponent(Entity &e, ComponentType &component) {
//		_pools[typeid(ComponentType)];
	}

}
