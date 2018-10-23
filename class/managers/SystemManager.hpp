//
// Created by Alexandre ROULIN on 22/10/2018.
//

#ifndef NIBBLER_SYSTEMMANAGER_HPP
#define NIBBLER_SYSTEMMANAGER_HPP

#include <vector>
#include <systems/ASystem.hpp>

class ECS;
class SystemManager {
private:
	std::vector<ASystem *>		_systems;
public:
	SystemManager() = delete;

	explicit SystemManager(ECS const &ecs);

	SystemManager &addToSystems(ASystem *);

	void registerEntity(Entity &e);
};

#endif //NIBBLER_SYSTEMMANAGER_HPP
