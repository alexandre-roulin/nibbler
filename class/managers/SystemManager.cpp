//
// Created by Alexandre ROULIN on 22/10/2018.
//

#include "SystemManager.hpp"

SystemManager::SystemManager(ECS const &ecs) {

}

SystemManager &SystemManager::addToSystems(ASystem *sys) {
	_systems.push_back(sys);
	return *this;
}

void SystemManager::registerEntity(Entity &e) {
	for (auto it = _systems.begin(); it != _systems.end(); ++it) {
		(*it)->registerEntity(e);
	}
}
