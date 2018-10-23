//
// Created by Alexandre ROULIN on 22/10/2018.
//

#ifndef NIBBLER_COMPONENTMANAGER_HPP
#define NIBBLER_COMPONENTMANAGER_HPP

#include <entity/Entity.hpp>
#include <map>
#include <typeindex>
#include <components/Component.hpp>
#include <managers/Pool.hpp>

class ECS;

class ComponentManager {
private:
	ECS const &ecs;
	std::map<std::type_index, Pool<Component<std::type_index>>> _pools;
public:
	ComponentManager() = delete;

	explicit ComponentManager(ECS const &ecs);

	template<typename ComponentType>
	void addComponent(Entity e, ComponentType c);

	template<typename ComponentType>
	ComponentType getComponent(Entity e);

};

#endif //NIBBLER_COMPONENTMANAGER_HPP
