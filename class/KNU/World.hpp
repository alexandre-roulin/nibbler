#pragma once

// >>> [ KRYSTAL is NOT UNIX ] <<<

#include <KNU/systems/System.hpp>
#include <KNU/events/EventManager.hpp>

namespace KNU {
	class Entity;

	class World {
	private:
		std::vector<Entity> _createdEntities;
		std::vector<Entity> _destroyedEntities;

		std::unique_ptr<EntitiesManager> _entityManager;
		std::unique_ptr<EventManager> _eventManager;
		std::unique_ptr<SystemManager> _systemManager;

	public:
		World();
		void update();
		Entity &createEntity();

		EntitiesManager &getEntityManager() const;

		EventManager &getEventManager() const;

		SystemManager &getSystemManager() const;

		void destroyEntity(Entity &e);
	};
}