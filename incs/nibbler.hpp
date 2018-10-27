#ifndef NIBBLER_NIBBLER_HPP
#define NIBBLER_NIBBLER_HPP

#define COMPONENT_MAX 5
#define BASE_ENTITIES_CAPACITY 20
#define GROUP_SNAKE "snake_group"
#define TAG_HEAD_SNAKE "snake_head"
#define TAG_TAIL_SNAKE "snake_tail"

enum eDirection {
	NONE,
	NORTH,
	SOUTH,
	EAST,
	WEST
};

enum eJoystick {
	ARROW_UP,
	ARROW_DOWN,
	ARROW_LEFT,
	ARROW_RIGHT,
	NONE
};

#endif //NIBBLER_NIBBLER_HPP
