#pragma once

// >>> [ KRYSTAL is NOT UNIX ] <<<

#include <vector>
#include <KNU/entities/Entity.hpp>
#include <KNU/managers/SystemManager.hpp>

namespace KNU {
	class Entity;
	class World {
	private:
		std::vector<Entity>		_createdEntities;
		std::vector<Entity>		_destroyedEntities;
		std::unique_ptr<EntitiesManager> _entityManager;
		std::unique_ptr<ComponentManager> _componentManager;
		std::unique_ptr<SystemManager> _systemManager;
	public:
		World();
		Entity &createEntity();

		ComponentManager &getComponentManager() const;
		EntitiesManager &getEntityManager() const;

		SystemManager &getSystemManager() const;
		void destroyEntity(Entity &e);
	};
}