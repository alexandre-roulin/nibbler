#pragma once

// >>> [ KRYSTAL is NOT UNIX ] <<<

#include <KNU/systems/System.hpp>
#include <KNU/events/EventManager.hpp>
#include "../../externlib/commun/IDisplay.hpp"

class Univers;

namespace KNU {
	class Entity;

	class World {
	private:
		std::vector<Entity> createdEntities_;
		std::vector<Entity> destroyedEntities_;
		std::unique_ptr<EntitiesManager> entityManager_;
		std::unique_ptr<EventManager> eventManager_;
		std::unique_ptr<SystemManager> systemManager_;
		IDisplay *display;
		int max_;


		friend class EntitiesManager;
	public:
		int getMax_() const;

		IDisplay *getDisplay() const;
		World(Univers &);

		void setDisplay(IDisplay *display);

		void update();

		Entity &createEntity();

		EntitiesManager &getEntityManager() const;

		EventManager &getEventManager() const;

		SystemManager &getSystemManager() const;

		void destroyEntity(Entity &e);
	};
}