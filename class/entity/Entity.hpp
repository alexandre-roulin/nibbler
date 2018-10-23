//
// Created by Alexandre ROULIN on 22/10/2018.
//

#ifndef NIBBLER_ENTITY_HPP
#define NIBBLER_ENTITY_HPP

//TAG
//GROUP

// <			=========			>
//SNAKE_HEAD	SNAKE_BODY			SNAKE_TAIL
//SNAKE

//std::vector<Entity>	getEntitiesByGroup(GROUP);
//Entity				getEntityByTag(TAG);

//Entity e = getEntityByTag(SNAKE_HEAD)
//createEntity()
//		.addC(Follow(e))


#include <components/ComponentMask.hpp>
#include <string>

struct Entity {
	Entity();

	explicit Entity(std::string &_tag) : _tag(_tag), id(0) {};

	explicit Entity(std::string &_tag, unsigned int id) : _tag(_tag), id(id) {};

	Entity &operator=(Entity const &);

	Entity(Entity const &);

	std::string _tag;
	unsigned int id;
	ComponentMask mask;
};

Entity::Entity(Entity const &e) {
	*this = e;
}

Entity &Entity::operator=(Entity const &e) {
	if (this != &e) {
		id = e.id;
		_tag = e._tag;
		mask.clear();
	}
	return *this;
}

Entity::Entity() {

}

#endif //NIBBLER_ENTITY_HPP
