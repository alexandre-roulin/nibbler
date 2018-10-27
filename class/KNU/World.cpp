#include <logger.h>
#include "World.hpp"

namespace KNU {

	World::World() {
		_entityManager = std::make_unique<EntitiesManager>(*this);
		_eventManager = std::make_unique<EventManager>(*this);
		_systemManager = std::make_unique<SystemManager>(*this);
	}

	Entity &World::createEntity() {
		auto &entity = _entityManager->createEntity();
		_createdEntities.push_back(entity);
		return entity;
	}

	EntitiesManager &World::getEntityManager() const {
		return *_entityManager;
	}

	SystemManager &World::getSystemManager() const {
		return *_systemManager;
	}

	EventManager &World::getEventManager() const {
		return *_eventManager;
	}
	void World::destroyEntity(Entity &e) {
		log_success("World::destroyEntity [%d]", e.getId());
		_destroyedEntities.push_back(e);
	}

// S 0000 0100

// E 0000 0111

// S 0000 0011
	void World::update() {
		for (auto &entity : _createdEntities) {
			_systemManager->addToSystems(entity);

		}
		_createdEntities.clear();
		for (auto &entity : _destroyedEntities) {
			_systemManager->removeToSystems(entity);
			_entityManager->destroyEntity(entity);
		}
		_destroyedEntities.clear();
		_eventManager->destroyEvents();
	}

}