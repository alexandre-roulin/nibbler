#include <logger.h>
#include "World.hpp"

namespace KNU {
	World::World(Univers &univers) : max_(30), grid(30, 30), univers_(univers) {

		entityManager_ = std::make_unique<EntitiesManager>(*this);
		eventManager_ = std::make_unique<EventManager>(*this);
		systemManager_ = std::make_unique<SystemManager>(*this);

	}

	Entity &World::createEntity() {
		auto &entity = entityManager_->createEntity();
		createdEntities_.push_back(entity);
		return entity;
	}

	EntitiesManager &World::getEntityManager() const {
		return *entityManager_;
	}

	SystemManager &World::getSystemManager() const {
		return *systemManager_;
	}

	EventManager &World::getEventManager() const {
		return *eventManager_;
	}
	void World::destroyEntity(Entity &e) {
		log_success("World::destroyEntity [%d]", e.getId());

		destroyedEntities_.push_back(e);
	}

	void World::update() {
		for (auto &entity : createdEntities_) {
			log_info("New id [%d] tag [%s] grps [%s]", entity.getId(), entity.getTag().c_str(), entity.getGroup().data());
			systemManager_->addToSystems(entity);
		}
		createdEntities_.clear();
		for (auto &entity : destroyedEntities_) {
			log_info("Dlt id [%d] tag [%s] grps [%s]", entity.getId(), entity.getTag().c_str(), entity.getGroup().data());
			systemManager_->removeToSystems(entity);
			entityManager_->destroyEntity(entity);
		}
		destroyedEntities_.clear();
		eventManager_->destroyEvents();
	}


	void World::setDisplay(IDisplay *display) {
		World::display = display;
	}

	IDisplay *World::getDisplay() const {
		return display;
	}

	int World::getMax_() const {
		return max_;
	}

	Univers &World::getUnivers() const {
		return univers_;
	}


}