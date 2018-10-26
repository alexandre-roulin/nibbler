#include "World.hpp"

namespace KNU {

	World::World() {
		_componentManager = std::make_unique<ComponentManager>();
		_entityManager = std::make_unique<EntitiesManager>(*this, *_componentManager);
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

	ComponentManager &
	World::getComponentManager() const {
		return *_componentManager;
	}

	void World::destroyEntity(Entity &e) {
		_destroyedEntities.push_back(e);
	}

	void World::update() {
		for (auto &entity : _createdEntities) {
			_systemManager->addToSystems(entity);
		}
		_createdEntities.clear();
		for (auto &entity : _destroyedEntities) {
			_systemManager->removeToSystems(entity);
		}
		_destroyedEntities.clear();
	}

}