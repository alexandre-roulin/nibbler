#include "ECS.hpp"

#include <managers/ComponentManager.hpp>
#include <managers/EntitiesManager.hpp>
#include <managers/SystemManager.hpp>

#include <memory>

ECS::ECS() {
	componentManager = std::make_unique<ComponentManager>(*this);
	entitiesManager = std::make_unique<EntitiesManager>(*this);
	systemManager =  std::make_unique<SystemManager>(*this);
}

const std::unique_ptr<ComponentManager> &ECS::getComponentManager() const {
	return componentManager;
}

const std::unique_ptr<SystemManager> &ECS::getSystemManager() const {
	return systemManager;
}

const std::unique_ptr<EntitiesManager> &ECS::getEntitiesManager() const {
	return entitiesManager;
}

void ECS::update() {

}
