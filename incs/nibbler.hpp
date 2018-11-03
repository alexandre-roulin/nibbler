#pragma once

#include <string>
#include <ostream>

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
	SNAKE_ARRAY,
	FOOD,
	ID,
};

struct		Snake
{
	Snake() : name(""), sprite(VOID), isReady(false), id(-1) {};

	std::string		name;
	eSnakeSprite	sprite;
	bool			isReady;
	int16_t				id;

	friend std::ostream &operator<<(std::ostream &os, const Snake &snake) {
		os << "name: " << snake.name << " sprite: " << snake.sprite
		   << " isReady: " << snake.isReady << " id: " << snake.id;
		return os;
	}

	Snake &operator=(Snake  const &snake) {
		if (this != &snake) {
			name = snake.name;
			sprite = snake.sprite;
			isReady = snake.isReady;
			id = snake.id;
		}
		return *this;
	}
};