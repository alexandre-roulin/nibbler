#include "World.hpp"

#include <KNU/managers/EntitiesManager.hpp>

namespace KNU {

	World::World() {
		entityManager = std::make_unique<EntitiesManager>(*this);
	}

	Entity &World::createEntity() {
		return entityManager->createEntity();
	}

}