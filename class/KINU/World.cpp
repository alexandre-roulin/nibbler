#include <logger.h>
#include "World.hpp"

namespace KINU {

	World::World(Univers &univers) : grid(univers.getMapSize()), univers_(univers) {
		grid.fill(FREE_SLOT);
		entitiesManager = std::make_unique<EntitiesManager>(*this);
		systemsManager = std::make_unique<SystemsManager>(*this);
		eventsManager = std::make_unique<EventsManager>(*this);
	}

	EntitiesManager &World::getEntitiesManager() const {
		assert(entitiesManager != nullptr);
		return *entitiesManager;
	}

	SystemsManager &World::getSystemsManager() const {
		assert(systemsManager != nullptr);
		return *systemsManager;
	}

	EventsManager &World::getEventsManager() const {
		assert(eventsManager != nullptr);
		return *eventsManager;
	}

	void World::update() {
		for (auto e : createdEntities) {
			getSystemsManager().addToSystems(e);
		}
		createdEntities.clear();

		for (auto e : destroyedEntities) {
			getSystemsManager().removeFromSystems(e);
			getEntitiesManager().destroyEntity(e);
		}
		destroyedEntities.clear();
	}

	Entity World::createEntity() {
		auto e = getEntitiesManager().createEntity();
		createdEntities.push_back(e);
		return e;
	}

	void World::destroyEntity(Entity e) {
		destroyedEntities.push_back(e);
	}

	Univers &World::getUnivers() const {
		return univers_;
	}

	void World::setDisplay(IDisplay *display) {
		display_ = display;
	}


}
