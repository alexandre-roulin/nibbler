#pragma once

#include <string>
#include <ostream>

#define MAX_SNAKE 8
#define MAX_COLOR 8
#define CHAT_BUFFER 128
#define NAME_BUFFER 22
#define OFFSET 8
#define SIZEOF_CHAT_PCKT CHAT_BUFFER + NAME_BUFFER + OFFSET
#define DEFAULT_SIZE_SPRITE 32

#define DIRECTION_VERTICAL 1			// 0000 0001
#define DIRECTION_HORIZONTAL 4			// 0000 0100

#define MAP_MAX 100
#define MAP_MIN 5

enum eTag {
	FOOD_TAG,
	FOOD_TAG_FROM_SNAKE,
	WALL_TAG,
	HEAD_TAG,
	BODY_TAG,
	TAIL_TAG,
};

enum class eSound {
	WELCOME = 0,
	READY = 2,
	RESIZE_MAP = 0,
	FOOD = 3,
	DEATH = 4
};

enum eDirection {
	NORTH = 1,							// 0000 0001
	SOUTH = 3,							// 0000 0011
	EAST = 4,							// 0000 0100
	WEST = 12							// 0000 1100
};

enum ePriority {
	NO_PRIORITY,
	MINOR_PRIORITY,
	SPECIFIC_LAST,
};

enum class eSprite {
	GREEN = 0,
	BLUE = 1,
	PURPLE = 2,
	PINK = 3,
	GREY = 4,
	YELLOW = 5,
	ORANGE = 6,
	RED = 7,

	MASK_COLOR = 0xFF,

	HEAD = (1 << 8),
	BODY = (1 << 9),
	TAIL = (1 << 10),

	MASK_BODY = HEAD | BODY | TAIL,

	NORTH = (1 << 11),
	SOUTH = (1 << 12),
	EAST = (1 << 13),
	WEST = (1 << 14),

	MASK_DIRECTION = NORTH | SOUTH | EAST | WEST,

	FROM_NORTH = (1 << 15),
	FROM_SOUTH = (1 << 16),
	FROM_EAST = (1 << 17),
	FROM_WEST = (1 << 18),

	MASK_FROM = FROM_NORTH | FROM_SOUTH | FROM_EAST | FROM_WEST,

	TO_NORTH = (1 << 19),
	TO_SOUTH = (1 << 20),
	TO_EAST = (1 << 21),
	TO_WEST = (1 << 22),

	MASK_TO = TO_NORTH | TO_SOUTH | TO_EAST | TO_WEST,

	BITWISE_TO = 8,
	BITWISE_FROM = 4,


	WALL = (1 << 23),
	FOOD = (1 << 24)
};
inline eSprite operator|(eSprite const x, eSprite const y) {
	return static_cast<eSprite> (static_cast<int>(x) | static_cast<int>(y));
}
inline eSprite const &operator|=(eSprite &x, eSprite const &y) {
	x = static_cast<eSprite> (static_cast<int>(x) | static_cast<int>(y));
	return (x);
}
inline eSprite operator&(eSprite const x, eSprite const y) {
	return static_cast<eSprite> (static_cast<int>(x) &  static_cast<int>(y));
}
inline eSprite operator&(int const x, eSprite const y) {
	return static_cast<eSprite> (x &  static_cast<int>(y));
}
inline eSprite operator&(eSprite const x, int const y) {
	return static_cast<eSprite> (static_cast<int>(x) & y);
}
inline eSprite operator^(int const x, eSprite const y) {
	return static_cast<eSprite> (x ^  static_cast<int>(y));
}
inline eSprite operator^(eSprite const x, int const y) {
	return static_cast<eSprite> (static_cast<int>(x) ^ y);
}
inline eSprite operator<<(eSprite const x, eSprite const y) {
	return static_cast<eSprite> (static_cast<int>(x) << static_cast<int>(y));
}
inline eSprite operator>>(eSprite const x, eSprite const y) {
	return static_cast<eSprite> (static_cast<int>(x) >> static_cast<int>(y));
}


struct		Snake
{
	Snake() : sprite(eSprite::BLUE), isReady(false), id(-1), isUpdate(false), direction(NORTH), isAlive(true) {
		bzero(name, NAME_BUFFER);
	};

	char			name[NAME_BUFFER];
	eSprite			sprite;
	bool			isReady;
	int16_t			id;
	bool			isUpdate;
	eDirection		direction;
	bool			isAlive;

	friend std::ostream &operator<<(std::ostream &os, const Snake &snake);

	Snake &operator=(Snake  const &snake) {
		if (this != &snake) {
			std::memcpy(name, snake.name, NAME_BUFFER);
			sprite = snake.sprite;
			isReady = snake.isReady;
			id = snake.id;
			isUpdate = snake.isUpdate;
			direction = snake.direction;
			isAlive = snake.isAlive;
		}
		return *this;
	}

	static Snake randomSnake(int16_t id) {
		Snake snake;

		snake.sprite = static_cast<eSprite>(rand() % MAX_COLOR);
		strncpy(snake.name, Snake::basicName[rand() % MAX_SNAKE].c_str(), NAME_BUFFER);
		snake.id = id;
		return (snake);
	}

	static int getlastSnakeIndex(Snake const *snakes, unsigned int range) {
		for (unsigned int i = 0; i < range; i++)
		{
			if (snakes[i].id == -1)
				return (i);
		}
		return (-1);
	}

	static int isFull(Snake const *snakes, unsigned int range) {
		for (unsigned int i = 0; i < range; i++)
		{
			if (snakes[i].id == -1)
				return (false);
		}
		return (true);
	}

	static int getSnakeById(Snake const *snakes, unsigned int range, int16_t id) {
		for (unsigned int i = 0; i < range; i++)
		{
			if (snakes[i].id == id)
				return (i);
		}
		return (-1);
	}
	
	static int allSnakesReady(Snake const *snakes, unsigned int range) {
		for (unsigned int i = 0; i < range; i++)
		{
			if (snakes[i].id != -1 && !snakes[i].isReady)
				return (false);
		}
		return (true);
	}

	static std::string const basicName[MAX_SNAKE];
};

//ADD SIZE IN ClientTCP::size_header

enum eHeader {
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
};
