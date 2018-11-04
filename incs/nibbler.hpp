#pragma once

#include <string>
#include <ostream>

#define MAX_SNAKE 8
#define MAX_SPRITE 3
#define CHAT_BUFFER 128
#define NAME_BUFFER 21
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
	GREEN,
	PURPLE,
	BLUE,
	VOID
};

struct		Snake
{
	Snake() : sprite(VOID), isReady(false), id(-1) {
		bzero(name, NAME_BUFFER + 1);
	};

	char			name[NAME_BUFFER + 1];
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
			strncpy(name, snake.name, NAME_BUFFER);
			sprite = snake.sprite;
			isReady = snake.isReady;
			id = snake.id;
		}
		return *this;
	}

	static Snake randomSnake(int16_t id) {
		Snake snake;

		snake.sprite = static_cast<eSnakeSprite>(rand() % VOID);
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

enum eHeader {
	CHAT,											//0
	FOOD,											//1
	ID = sizeof(int16_t),							//2
	SNAKE = sizeof(Snake),							//32
	SNAKE_ARRAY = sizeof(Snake) * MAX_SNAKE,		//256
};
