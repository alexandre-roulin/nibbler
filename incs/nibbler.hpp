#pragma once

#include <string>

#define MAX_SNAKE 8
#define CHAT_BUFFER 128
#define COMPONENT_MAX 7
#define BASE_ENTITIES_CAPACITY 20
#define GROUP_SNAKE "snake_group"
#define TAG_HEAD_SNAKE "snake_head"
#define TAG_TAIL_SNAKE "snake_tail"

enum eDirection {
	NORTH,
	SOUTH,
	EAST,
	WEST
};

enum eJoystick {
	NONE,
	ARROW_UP,
	ARROW_DOWN,
	ARROW_LEFT,
	ARROW_RIGHT
};


enum eSnakeSprite {
	VOID,
	GREEN,
	RED
};

enum eHeader {
	CHAT,
	SNAKE,
	FOOD
};

typedef struct		s_snake
{
	std::string		name;
	eSnakeSprite	sprite;
	bool			isReady;
	unsigned int	id;
}					t_snake;
