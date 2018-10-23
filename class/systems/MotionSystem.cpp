//
// Created by Alexandre ROULIN on 23/10/2018.
//

#include "MotionSystem.hpp"

void MotionSystem::update() {

}

template<typename ComponentType>
void MotionSystem::addSignature() {
	cm.addComponent<ComponentType>();
}

template<typename ComponentType>
void MotionSystem::removeSignature() {
	cm.removeComponent<ComponentType>();
}

MotionSystem::~MotionSystem() {

}

void MotionSystem::registerEntity(Entity &e) {
	if (cm == e.mask)
		_registered_entities.push_back(e);
}

void MotionSystem::unregisterEntity(Entity &e) {
	for (auto it = _registered_entities.begin();
		 it != _registered_entities.end(); ++it) {
		(*it).id = e.id;
		_registered_entities.erase(it);
	}
}
