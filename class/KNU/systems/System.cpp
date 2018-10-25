
#include "System.hpp"
#include <KNU/entities/Entity.hpp>

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

	std::vector <Entity> System::getEntities() {
		return entities;
	}

	template<typename T>
	void System::requireComponent() {
		signature.addComponent<T>();
	}


}