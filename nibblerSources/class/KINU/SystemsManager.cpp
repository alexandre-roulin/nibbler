#include "SystemsManager.hpp"
#include "World.hpp"

namespace KINU {

	std::vector<Entity> System::getEntities() {
		return entities;
	}

	void System::addEntity(Entity entity) {
		entities.push_back(entity);
	}

	void System::removeEntity(Entity e) {
		entities.erase(std::remove_if(entities.begin(),
									  entities.end(),
									  [&e](Entity other) {
										  return e == other;
									  }), entities.end());
	}

	World &System::getWorld() const {
		return *world;
	}

	const ComponentMask &System::getComponentMask() const {
		return componentMask;
	}


	void SystemsManager::addToSystems(Entity e) {
		const auto &entityComponentMask = world.getEntitiesManager().getComponentMask(
				e);

		for (auto &it : systems) {
			auto &system = it.second;
			const auto &systemComponentMask = system->getComponentMask();
			auto interest = (entityComponentMask & systemComponentMask) ==
							systemComponentMask;

			if (interest) {
				system->addEntity(e);
			}
		}
	}

	void SystemsManager::removeFromSystems(Entity e) {
		for (auto &it : systems) {
			auto &system = it.second;
			system->removeEntity(e);
		}
	}

}