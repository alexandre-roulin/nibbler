#pragma once

#include <string>
#include <ostream>
#include "IDisplay.hpp"

#define MAX_SNAKE 8
#define MAX_COLOR 8
#define CHAT_BUFFER 128
#define NAME_BUFFER 22
#define OFFSET 8
#define SIZEOF_CHAT_PCKT CHAT_BUFFER + NAME_BUFFER + OFFSET
#define DEFAULT_SIZE_SPRITE 32

#define DIRECTION_VERTICAL 1            // 0000 0001
#define DIRECTION_HORIZONTAL 4            // 0000 0100

#define MAP_MIN 5
#define MAP_DEFAULT 30
#define MAP_MAX 100

enum eTag {
	HEAD_TAG = 0,
	TAIL_TAG = 8,
	FOOD_TAG = 16,				//16
	FOOD_TAG_FROM_SNAKE,		//17
	WALL_TAG,					//18
};

enum class eSound {
	WELCOME = 0,
	READY = 2,
	RESIZE_MAP = 0,
	FOOD = 3,
	DEATH = 4
};


enum ePriority {
	NO_PRIORITY,
	MINOR_PRIORITY,
	SPECIFIC_LAST,
};

struct Snake {
	Snake() : sprite(eSprite::BLUE), isReady(false), id(-1), isUpdate(false),
			  direction(NORTH), isAlive(true) {
		bzero(name, NAME_BUFFER);
	};

	char name[NAME_BUFFER];
	eSprite sprite;
	bool isReady;
	int16_t id;
	bool isUpdate;
	eDirection direction;
	bool isAlive;
	uint16_t score;

	friend std::ostream &operator<<(std::ostream &os, const Snake &snake);


	Snake &operator=(Snake const &snake) {
		if (this != &snake) {
			std::memcpy(name, snake.name, NAME_BUFFER);
			sprite = snake.sprite;
			isReady = snake.isReady;
			id = snake.id;
			isUpdate = snake.isUpdate;
			direction = snake.direction;
			isAlive = snake.isAlive;
			score = snake.score;
		}
		return *this;
	}

	static Snake randomSnake(int16_t id) {
		Snake snake;

		snake.sprite = static_cast<eSprite>(static_cast<int>(eSprite::GREEN) + rand() % MAX_COLOR);
		strncpy(snake.name, Snake::basicName[rand() % MAX_SNAKE].c_str(),
				NAME_BUFFER);
		snake.id = id;
		return (snake);
	}

	static int getlastSnakeIndex(Snake const *snakes, unsigned int range) {
		unsigned int i;
		for (i = 0; i < range; ++i) {
			if (snakes[i].id == -1)
				return (i);
		}
		return (i == range ? i - 1 : -1);
	}

	static int isFull(Snake const *snakes, unsigned int range) {
		for (unsigned int i = 0; i < range; i++) {
			if (snakes[i].id == -1)
				return (false);
		}
		return (true);
	}

	static int
	getSnakeById(Snake const *snakes, unsigned int range, int16_t id) {
		unsigned int i;
		for (i = 0; i < range; i++) {
			if (snakes[i].id == id)
				return (i);
		}
		return (i == range ? i - 1 : -1);
	}

	static int allSnakesReady(Snake const *snakes, unsigned int range) {
		for (unsigned int i = 0; i < range; i++) {
			if (snakes[i].id != -1 && !snakes[i].isReady)
				return (false);
		}
		return (true);
	}

	static std::string const basicName[MAX_SNAKE];
};

//ADD SIZE IN ClientTCP::size_header

enum class eHeader {
	CHAT,				//0
	FOOD,				//1
	ID,					//2
	OPEN_GAME,			//3
	START_GAME,			//4
	SNAKE,				//5
	SNAKE_ARRAY,		//6
	HEADER,				//7
	INPUT,				//8
	RESIZE_MAP,			//9
	REMOVE_SNAKE,		//10
	POCK,				//11
	BORDERLESS,			//12
	DISCONNECT          //13
};
