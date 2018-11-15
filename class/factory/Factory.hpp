#pragma once

#define PART_SNAKE 4
#include <nibbler.hpp>
#include <KINU/World.hpp>
#include <KINU/Entity.hpp>
#include <component/CollisionComponent.hpp>
#include <component/JoystickComponent.hpp>
#include <component/MotionComponent.hpp>
#include <component/PositionComponent.hpp>
#include <component/FollowComponent.hpp>

enum eSnakePart {
	HEAD,						// 0
	BODY,						// 1
	TAIL,						// 2
	GRPS,						// 3
	OTHER						// 4
};

class Factory {
public:
	void create_all_snake(Snake snake_array[8], int16_t nu);
	void create_snake(int16_t , int, int);
	void create_food(int, int);
	static std::string factory_name(eSnakePart, int16_t id);
	static KINU::Entity::Id getIdFromTag(std::string tag);
	static char * factory_chat_message(char const *name, char const *message);
	static eSnakePart isSnakePart(std::string );
	Factory(Univers &univers);

private:
	void create_walls();
	void create_wall(int x, int y);
	static const char *part_name[PART_SNAKE];
	Univers &univers_;
};
