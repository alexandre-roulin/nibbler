#pragma once

#define PART_SNAKE 4
#include <nibbler.hpp>
#include <KNU/World.hpp>
#include <KNU/entities/Entity.hpp>
#include <component/CollisionComponent.hpp>
#include <component/JoystickComponent.hpp>
#include <component/MotionComponent.hpp>
#include <component/PositionComponent.hpp>
#include <component/FollowComponent.hpp>

enum eSnakePart {
	HEAD,
	BODY,
	TAIL,
	GRP,
	OTHER
};

class Factory {
public:
	void create_all_snake(Snake snake_array[8], int16_t nu);
	void create_snake(int16_t , int, int);
	void create_food(int, int);
	static char * factory_name(eSnakePart, int16_t id);
	static char * factory_pseudo(char const *name);
	static char * factory_chat_message(char const *name, char const *message);
	static std::string factory_name(eSnakePart, std::string);
	static eSnakePart isSnakePart(std::string );
	Factory(KNU::World &world);

private:
	void create_walls();
	void create_wall(int x, int y);
	static const char *part_name[PART_SNAKE];
	KNU::World &world_;
};
