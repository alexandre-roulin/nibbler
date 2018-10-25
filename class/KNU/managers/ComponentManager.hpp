#pragma once

#include <map>
#include <typeindex>
#include "Pool.hpp"
#include <KNU/entities/Entity.hpp>
#include <iostream>
#include <KNU/component/Component.hpp>
#include <nibbler.hpp>

namespace KNU {

	class ComponentManager {
	private:
		std::vector<std::shared_ptr<AbstractPool>> componentPools;

	public:
		ComponentManager() {
			componentPools.resize(COMPONENT_MAX);
		};

		template<typename T>
		void addComponent(Entity e, T component) {
			assert(e.mask.matches(component.mask));
			const auto componentId = Component<T>::signature();
			std::shared_ptr<Pool<T>> componentPool = accommodateComponent<T>();
			componentPool->add(e, component);
		}

		template<typename T>
		T &getComponent(Entity &entity) {
			std::shared_ptr<Pool<T>> componentPool = accommodateComponent<T>();
			return componentPool->get(entity);
		}

		template<typename T>
		void removeComponent(Entity &entity) {
			std::shared_ptr<Pool<T>> componentPool = accommodateComponent<T>();
			componentPool->remove(entity);
		}
		template<typename T>
		bool hasComponent(Entity &entity) {
			std::shared_ptr<Pool<T>> componentPool = accommodateComponent<T>();
			return componentPool->has(entity);
		}

		template<typename T>
		std::shared_ptr<Pool<T>>
		accommodateComponent() {
			const auto componentId = Component<T>::signature();
			assert(componentId < COMPONENT_MAX);

			if (!componentPools[componentId]) {
				std::shared_ptr<Pool<T>> pool(new Pool<T>());
				componentPools[componentId] = pool;
			}

			return std::static_pointer_cast<Pool<T>>(
					componentPools[componentId]);

		}
	};


}
