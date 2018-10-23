//
// Created by Alexandre ROULIN on 23/10/2018.
//

#ifndef NIBBLER_ENTITYFACTORY_HPP
#define NIBBLER_ENTITYFACTORY_HPP


#include <entity/Entity.hpp>
#include <nibbler.hpp>
class ECS;

class EntityFactory {
public:
	EntityFactory() = delete;

	explicit EntityFactory(ECS const &ecs);

	EntityFactory &createEntity(std::string tag);

	template <typename ComponentType>
	EntityFactory &addComponent(ComponentType const &c);

	Entity &build();

private:
	Entity e;
	ECS const &ecs;
};


#endif //NIBBLER_ENTITYFACTORY_HPP

									//EMBER { MOVE | JOYSTICK }
					//													\\
//SWORD1 { MOVE | FOLLOW{EMBER} }								//SWORD2 { MOVE | FOLLOW{EMBER} }
//					||													||
//SWORD_EFFECT { MOVE | FOLLOW{SWORD1} }						//SWORD_EFFECT { MOVE | FOLLOW{SWORD2} }


