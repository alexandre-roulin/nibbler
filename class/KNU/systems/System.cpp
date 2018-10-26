
#include <logger.h>
#include "System.hpp"

namespace KNU {

	void System::addEntity(Entity &entity) {
		assert(entity.getSignature().matches(signature));
		entities.push_back(entity);
	}

	void System::removeEntity(Entity entity) {
		entities.erase(
				std::remove_if(
						entities.begin(),
						entities.end(),
						[&entity](Entity other) {
							return entity == other;
						}),
				entities.end());
	}

	World &System::getWorld() const {
		return *world;
	}

	const Signature &System::getSignature() const {
		return signature;
	}

	std::vector<Entity> System::getEntities() {
		return entities;
	}


	void SystemManager::addToSystems(Entity &entity) {
		for (auto &systemMap : _systems) {
			auto system = systemMap.second;

			if (entity.getSignature().matches(system->getSignature())) {
				system->addEntity(entity);
			}
		}
	}

	SystemManager::SystemManager(World &world)
			: _world(world) {

	}

	void SystemManager::removeToSystems(Entity &entity) {
		for (auto &systemMap : _systems) {
			auto system = systemMap.second;
			if (entity.getSignature().matches(system->getSignature()))
				system->removeEntity(entity);
		}
	}


}