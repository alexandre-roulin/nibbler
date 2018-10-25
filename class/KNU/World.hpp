#pragma once

// >>> [ KRYSTAL is NOT UNIX ] <<<
#include <KNU/managers/EntitiesManager.hpp>
#include <KNU/managers/SystemManager.hpp>

namespace KNU {
	class EntitiesManager;
	class SystemManager;
	class World {
	private:
		std::vector<Entity>		_createdEntities;
		std::vector<Entity>		_destroyedEntities;
		std::unique_ptr<EntitiesManager> entityManager = nullptr;
		std::unique_ptr<SystemManager> systemManager = nullptr;
//		std::unique_ptr<EventManager> eventManager = nullptr;
	public:
		World();
		Entity &createEntity();

	};
}