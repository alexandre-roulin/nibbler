#ifndef PROJECT_SNAKE_HPP
#define PROJECT_SNAKE_HPP

#include <cstdio>
#include <Sprite.hpp>
#include <nibbler.hpp>

struct Snake {
public:
	Snake();

	char name[NAME_BUFFER];
	eSprite sprite;
	bool isReady;
	int16_t id;
	bool isUpdate;
	eDirection direction;
	bool isAlive;
	bool isIA;
	bool isSwitchingLibrary;
	size_t indexConnection;
	bool isValid;

	Snake &operator=(Snake const &snake);

	static Snake randomSnake(int16_t id);

	static std::string const basicName[SNAKE_MAX_NAME];
};






#endif //PROJECT_SNAKE_HPP
