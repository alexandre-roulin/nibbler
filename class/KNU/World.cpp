#include "World.hpp"

namespace KNU {

	World::World() {
		_entityManager = std::make_unique<EntitiesManager>(*this, *_componentManager);
		_systemManager = std::make_unique<SystemManager>(*this);
	}

	Entity &World::createEntity() {
		return _entityManager->createEntity();
	}

	EntitiesManager &World::getEntityManager() const {
		return *_entityManager;
	}

	SystemManager &World::getSystemManager() const {
		return *_systemManager;
	}

	ComponentManager &
	World::getComponentManager() const {
		return *_componentManager;
	}

	void World::destroyEntity(Entity &e) {
		_destroyedEntities.push_back(e);
	}

}