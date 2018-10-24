#pragma once

#include <map>
#include <typeindex>
#include "Pool.hpp"
#include <KNU/entities/Entity.hpp>
#include <iostream>
#include <KNU/component/Component.hpp>

namespace KNU {

	class ComponentManager {
	private:
		std::vector<std::shared_ptr<AbstractPool>> componentPools;

	public:
		ComponentManager() = default;

		template<typename T>
		void addComponent(Entity e, T component) {
			const auto componentId = Component<T>::signature();
			std::shared_ptr<Pool<T>> componentPool = accommodateComponent<T>();
		}

		template<typename T>
		std::shared_ptr<Pool<T>>
		accommodateComponent() {
			const auto componentId = Component<T>::signature();

			if (componentId >= componentPools.size()) {
				componentPools.resize(componentId + 1, nullptr);
			}

			if (!componentPools[componentId]) {
				std::shared_ptr<Pool<T>> pool(new Pool<T>());
				componentPools[componentId] = pool;
			}

			return std::static_pointer_cast<Pool<T>>(
					componentPools[componentId]);

		}
	};


}
