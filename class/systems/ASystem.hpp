//
// Created by Alexandre ROULIN on 23/10/2018.
//

#ifndef NIBBLER_ISYSTEM_HPP
#define NIBBLER_ISYSTEM_HPP

#include <components/ComponentMask.hpp>
#include <vector>
#include <entity/Entity.hpp>

class ASystem {
protected:
	std::vector<Entity> _registered_entities;
public:
	ComponentMask	cm;
	virtual void registerEntity(Entity &e) = 0;

	virtual void unregisterEntity(Entity &e) = 0;
	virtual void update() = 0;
	virtual ~ASystem() = default;;
};

#endif //NIBBLER_ISYSTEM_HPP
