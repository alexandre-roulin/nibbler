
#include "System.hpp"

void KNU::System::addEntity(Entity e) {
	assert(e.getMask().matches(signature));

}

void KNU::System::removeEntity(KNU::Entity e) {
	entities.erase(
			std::remove_if(
					entities.begin(),
					entities.end(),
					[&e](Entity other) { return e == other; }),
					entities.end());
}

KNU::World &KNU::System::getWorld() const {
	return *world;
}

const KNU::Signature &KNU::System::getSignature() const {
	return signature;
}

std::vector<KNU::Entity> KNU::System::getEntities() {
	return entities;
}

