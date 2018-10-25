#pragma once

// >>> [ KRYSTAL is NOT UNIX ] <<<

#include <vector>
#include <KNU/managers/ComponentManager.hpp>
#include <KNU/managers/EntitiesManager.hpp>

namespace KNU {
	class Entity;
	class World {
	private:
		std::vector<Entity>		_createdEntities;
		std::vector<Entity>		_destroyedEntities;
		std::unique_ptr<EntitiesManager> entityManager;
		std::unique_ptr<ComponentManager> componentManager;
//		std::unique_ptr<EventManager> eventManager = nullptr;
	public:
		World();
		Entity &createEntity();

		ComponentManager &getComponentManager() const;
		EntitiesManager &getEntityManager() const;

	};
}