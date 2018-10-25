
#include <KNU/component/Component.hpp>
#include "ComponentManager.hpp"
#include <nibbler.hpp>
namespace KNU {


	template<typename T>
	void ComponentManager::addComponent(Entity &e, T component) {
		const auto componentId = Component<T>::signature();
		std::shared_ptr<Pool<T>> componentPool = accommodateComponent<T>();
		componentPool->add(e, component);
	}

	template<typename T>
	T &ComponentManager::getComponent(Entity &entity) {
		std::shared_ptr<Pool<T>> componentPool = accommodateComponent<T>();
		return componentPool->get(entity);
	}

	template<typename T>
	void ComponentManager::removeComponent(Entity &entity) {

		std::shared_ptr<Pool<T>> componentPool = accommodateComponent<T>();
		componentPool->remove(entity);
	}

	template<typename T>
	bool ComponentManager::hasComponent(Entity const &entity) {
		std::shared_ptr<Pool<T>> componentPool = accommodateComponent<T>();
		return componentPool->has(entity);
	}

	template<typename T>
	std::shared_ptr<Pool<T>> ComponentManager::accommodateComponent() {

		const auto componentId = Component<T>::signature();
		assert(componentId < COMPONENT_MAX);

		if (!componentPools[componentId]) {
			std::shared_ptr<Pool<T>> pool(new Pool<T>());
			componentPools[componentId] = pool;
		}

		return std::static_pointer_cast<Pool<T>>(
				componentPools[componentId]);

	}

	KNU::ComponentManager::ComponentManager() {
		componentPools.resize(COMPONENT_MAX);
	}

	ComponentManager::ComponentManager(ComponentManager const &) {

	}
}