//
// Created by Alexandre ROULIN on 23/10/2018.
//

#ifndef NIBBLER_ENTITIESMANAGER_HPP
#define NIBBLER_ENTITIESMANAGER_HPP


#include <entity/Entity.hpp>
#include <ECS.hpp>
#include <vector>
#include <factories/EntityFactory.hpp>


class EntitiesManager {
	ECS const & ecs;
	std::vector<Entity>		_entities;
	std::unique_ptr<EntityFactory> entityFactory;

public:
	EntitiesManager() = delete;

	explicit EntitiesManager(ECS const &ecs);

	Entity createEntity(std::string &tag);

	const std::unique_ptr<EntityFactory> &getEntityFactory() const;

	template<typename ComponentType>
	bool addComponent(Entity const &e, ComponentType const &c);
};


#endif //NIBBLER_ENTITIESMANAGER_HPP
