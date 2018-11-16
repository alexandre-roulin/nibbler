#pragma once

#include <string>
#include <ostream>

#define MAX_SNAKE 8
#define CHAT_BUFFER 128
#define NAME_BUFFER 22
#define OFFSET 8
#define SIZEOF_CHAT_PCKT CHAT_BUFFER + NAME_BUFFER + OFFSET
#define WALL_TAG "wall"
#define FOOD_TAG "food"

#define COMPONENT_MAX 6
#define BASE_ENTITIES_CAPACITY 20
#define MAP_MAX		20
#define MAP_MIN		8
#define SIZE_LINE_TILESET 15

#define DIRECTION_VERTICAL 1			// 0000 0001
#define DIRECTION_HORIZONTAL 4			// 0000 0100

enum eDirection {
	NORTH = 1,							// 0000 0001
	SOUTH = 3,							// 0000 0011
	EAST = 4,							// 0000 0100
	WEST = 12							// 0000 1100
};

//int  0000 0000	0000 0000	0000 0000	0000 0010

//(motionComponent.direction & DIRECTION_VERTICAL &&
//					!(joystickComponent.direction & DIRECTION_VERTICAL))

//MOTION_COMPONENT		0000 0001

//JOYSTICK_COMPONENT	0000 0011
//						&
//DIRECTION_VERTICAL	0000 0001

enum eSprite {
	GREEN_SNAKE,
	BLUE_SNAKE,
	PURPLE_SNAKE,
	PINK_SNAKE,
	GREY_SNAKE,
	YELLOW_SNAKE,
	ORANGE_SNAKE,
	RED_SNAKE,
	VARIOUS
};

struct		Snake
{
	Snake() : sprite(VOID), isReady(false), id(-1) {
		bzero(name, NAME_BUFFER);
	};

	char			name[NAME_BUFFER];
	eSprite	sprite;
	bool			isReady;
	int16_t				id;

	friend std::ostream &operator<<(std::ostream &os, const Snake &snake) {
		os << " sprite: " << snake.sprite
		   << " isReady: " << snake.isReady << " id: " << snake.id;
		return os;
	}

	Snake &operator=(Snake  const &snake) {
		if (this != &snake) {
			std::memcpy(name, snake.name, NAME_BUFFER);
			sprite = snake.sprite;
			isReady = snake.isReady;
			id = snake.id;
		}
		return *this;
	}

	static Snake randomSnake(int16_t id) {
		Snake snake;

		snake.sprite = static_cast<eSprite>(rand() % VOID);
		strncpy(snake.name, Snake::basicName[rand() % MAX_SNAKE].c_str(), NAME_BUFFER);
		snake.id = id;
		return (snake);
	}

	static int getlastSnakeIndex(Snake *snakes, unsigned int range) {
		for (unsigned int i = 0; i < range; i++)
		{
			if (snakes[i].id == -1)
				return (i);
		}
		return (-1);
	}

	static int isFull(Snake *snakes, unsigned int range) {
		for (unsigned int i = 0; i < range; i++)
		{
			if (snakes[i].id == -1)
				return (false);
		}
		return (true);
	}

	static int getSnakeById(Snake *snakes, unsigned int range, int16_t id) {
		for (unsigned int i = 0; i < range; i++)
		{
			if (snakes[i].id == id)
				return (i);
		}
		return (-1);
	}

	static std::string const basicName[MAX_SNAKE];
};

//ADD SIZE IN ClientTCP::size_header

enum eHeader {
	CHAT,				//0
	FOOD,				//1
	ID,					//2
	START_GAME,			//3
	SNAKE,				//4
	SNAKE_ARRAY,		//5
	HEADER,				//6
	INPUT,				//7
};
