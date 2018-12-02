#pragma once

#include <nibbler.hpp>
#include <KINU/World.hpp>
#include <KINU/Entity.hpp>
#include <component/CollisionComponent.hpp>
#include <component/JoystickComponent.hpp>
#include <component/MotionComponent.hpp>
#include <component/PositionComponent.hpp>
#include <component/FollowComponent.hpp>

class Factory {
public:
	void create_all_snake(Snake snake_array[8], int16_t nu);

	void create_snake(Snake, int);

	static char *factory_chat_message(char const *name, char const *message);

	Factory(Univers &univers);

private:
	void create_walls();

	void create_wall(int x, int y);
	Univers &univers_;
};
