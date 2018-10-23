//
// Created by Alexandre ROULIN on 23/10/2018.
//

#include "EntitiesManager.hpp"
#include "ComponentManager.hpp"

template<typename ComponentType>
bool EntitiesManager::addComponent(Entity const &e, const ComponentType &c) {
	if (e.mask & c.signature) {
		ecs.getComponentManager()->addComponent<ComponentType>(e, c);
		return true;
	}
	return false;
}

Entity EntitiesManager::createEntity(std::string &tag) {
	return Entity(tag);
}

EntitiesManager::EntitiesManager(ECS const &ecs) : ecs(ecs) {
	entityFactory =  std::make_unique<EntityFactory>(ecs);

}

const std::unique_ptr<EntityFactory> &
EntitiesManager::getEntityFactory() const {
	return entityFactory;
}


