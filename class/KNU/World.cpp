#include "World.hpp"

namespace KNU {

	World::World() {
		entityManager = std::make_unique<EntitiesManager>(*this, *componentManager);
//		systemManager = std::make_unique<SystemManager>(*this);
	}

	Entity &World::createEntity() {
		return entityManager->createEntity();
	}

	EntitiesManager &World::getEntityManager() const {
		return *entityManager;
	}

	ComponentManager &
	World::getComponentManager() const {
		return *componentManager;
	}

}