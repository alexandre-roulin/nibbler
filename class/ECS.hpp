//
// Created by Alexandre ROULIN on 21/10/2018.
//

#ifndef NIBBLER_ECS_HPP
#define NIBBLER_ECS_HPP

class EntitiesManager;

class SystemManager;

#include <memory>
#include <managers/ComponentManager.hpp>

class ECS {
private:
	std::unique_ptr<ComponentManager> componentManager;
	std::unique_ptr<SystemManager> systemManager;
	std::unique_ptr<EntitiesManager> entitiesManager;

public:
	const std::unique_ptr<ComponentManager> &getComponentManager() const;

	const std::unique_ptr<SystemManager> &getSystemManager() const;

	const std::unique_ptr<EntitiesManager> &getEntitiesManager() const;

	void update();

	ECS();
};

#endif //NIBBLER_ECS_HPP
