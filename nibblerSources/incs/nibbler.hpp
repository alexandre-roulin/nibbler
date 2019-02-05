#pragma once

#include <string>
#include <ostream>
#include "IDisplay.hpp"

#ifndef NIBBLER_ROOT_PROJECT_PATH
	#define NIBBLER_ROOT_PROJECT_PATH 0
#endif

#define SNAKE_MAX 8
#define SNAKE_MAX_NAME 8
#define SNAKE_MAX_COLOR 8
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
	FOOD_TAG = 16,
	FOOD_TAG_FROM_SNAKE = 32,
	WALL_TAG = 64,
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
	Snake() :
	sprite(eSprite::BLUE),
	isReady(false),
	id(-1),
	isUpdate(false),
	direction(kNorth),
	isAlive(false),
	isSwitchingLibrary(false) {
		memset(name, 0, NAME_BUFFER);
	};

	char name[NAME_BUFFER];
	eSprite sprite;
	bool isReady;
	int16_t id;
	bool isUpdate;
	eDirection direction;
	bool isAlive;
	bool isSwitchingLibrary;
	size_t indexConnection;
	friend std::ostream &operator<<(std::ostream &os, const Snake &snake);

	void reset() {
		memset(name, 0, NAME_BUFFER);
		sprite = eSprite::GREEN;
		isReady = false;
		id = -1;
		isUpdate = false;
		direction = kNorth;
		isAlive = false;
		isSwitchingLibrary = false;
		indexConnection = -1;
	}

	Snake &operator=(Snake const &snake) {
		if (this != &snake) {
			std::memcpy(name, snake.name, NAME_BUFFER);
			sprite = snake.sprite;
			isReady = snake.isReady;
			id = snake.id;
			isUpdate = snake.isUpdate;
			direction = snake.direction;
			isAlive = snake.isAlive;
			isSwitchingLibrary = snake.isSwitchingLibrary;
			indexConnection = snake.indexConnection;
		}
		return *this;
	}

	static Snake randomSnake(int16_t id) {
		Snake snake;

		snake.sprite = static_cast<eSprite>(static_cast<int>(eSprite::GREEN) + rand() % SNAKE_MAX_COLOR);
		strncpy(snake.name, Snake::basicName[rand() % SNAKE_MAX_NAME].c_str(), NAME_BUFFER);
		snake.id = id;
		snake.isAlive = true;
		snake.isSwitchingLibrary = false;
		return (snake);
	}

	static int allSnakesReady(Snake const *snakes) {
		for (unsigned int i = 0; i < SNAKE_MAX; i++) {
			if (snakes[i].id != -1 && !snakes[i].isReady)
				return (false);
		}
		return (true);
	}

	static std::string const basicName[SNAKE_MAX_NAME];
};

//ADD SIZE IN ClientTCP::size_header

enum class eHeader {
	CHAT,
	FOOD,
	ID,
	OPEN_GAME,
	START_GAME,
	SNAKE,
	HEADER,
	INPUT,
	RESIZE_MAP,
	REMOVE_SNAKE,
	POCK,
	BORDERLESS,
	kPause,
	kForcePause
};
