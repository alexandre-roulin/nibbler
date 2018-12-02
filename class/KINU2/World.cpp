#include <logger.h>
#include "World.hpp"

namespace KINU {

	World::World(Univers &univers) : grid(0), univers_(univers){
		entityManager = std::make_unique<EntityManager>(*this);
		systemManager = std::make_unique<SystemManager>(*this);
		eventManager = std::make_unique<EventManager>(*this);
		log_success("New map create [%d %d]",univers.getMapSize(),univers.getMapSize());
	}

	EntityManager &World::getEntityManager() const {
		assert(entityManager != nullptr);
		return *entityManager;
	}

	SystemManager &World::getSystemManager() const {
		assert(systemManager != nullptr);
		return *systemManager;
	}

	EventManager &World::getEventManager() const {
		assert(eventManager != nullptr);
		return *eventManager;
	}

	void World::update() {
		for (auto e : createdEntities) {
//			log_info("Created entity index[%d] ", e.getIndex());
//			log_info("Created entity t[%s]", e.getTag().c_str());
//			log_info("Created entity g[%s]", e.getGroup().c_str());
			getSystemManager().addToSystems(e);
		}
		createdEntities.clear();

		for (auto e : destroyedEntities) {
			getSystemManager().removeFromSystems(e);
			getEntityManager().destroyEntity(e);
		}
		destroyedEntities.clear();
	}

	Entity World::createEntity() {
		auto e = getEntityManager().createEntity();
		createdEntities.push_back(e);
		return e;
	}

	void World::destroyEntity(Entity e) {
		destroyedEntities.push_back(e);
	}

	Entity World::getEntity(std::string tag) const {
		return getEntityManager().getEntityByTag(tag);
	}

	std::vector<Entity> World::getGroup(std::string group) const {
		return getEntityManager().getEntityGroup(group);
	}

	Univers &World::getUnivers() const {
		return univers_;
	}

	void World::setDisplay(IDisplay *display) {
		display_ = display;
	}


}
