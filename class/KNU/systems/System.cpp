
#include "System.hpp"

namespace KNU {

	void System::addEntity(Entity e) {
		assert(e.getMask().matches(signature));

	}

	void System::removeEntity(Entity e) {
		entities.erase(
				std::remove_if(
						entities.begin(),
						entities.end(),
						[&e](Entity other) { return e == other; }),
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
		std::cout << "SystemManager::addToSystems"  << _systems.size() << std::endl;
		for (auto &systemMap : _systems) {
			auto system = systemMap.second;
			std::cout << "e : " << entity.getMask() << "sys : " << system->getSignature() <<  std::endl;

			if (entity.getMask().matches(system->getSignature())) {
				system->addEntity(entity);
			}
		}
	}

	SystemManager::SystemManager(World &world)
			: _world(world) {

	}

	void SystemManager::removeToSystems(Entity & entity) {
		for (auto &systemMap : _systems) {
			auto system = systemMap.second;
			if (entity.getMask().matches(system->getSignature()))
				system->removeEntity(entity);
		}	}


}