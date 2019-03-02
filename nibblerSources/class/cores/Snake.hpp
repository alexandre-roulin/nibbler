#ifndef PROJECT_SNAKE_HPP
#define PROJECT_SNAKE_HPP

#include <cstdio>
#include <Sprite.hpp>
#include <nibbler.hpp>
#include <ostream>

class SnakeUI {
public:
	SnakeUI();

	virtual SnakeUI &operator=(SnakeUI &);

	virtual SnakeUI &operator=(SnakeUI const &);

	char name[NAME_BUFFER];

	friend std::ostream &operator<<(std::ostream &os, const SnakeUI &ui);

	eSprite sprite;
	bool isReadyToExpose;
};

class SnakeUX {
public:
	friend std::ostream &operator<<(std::ostream &os, const SnakeUX &ux);

	SnakeUX();

	virtual SnakeUX &operator=(SnakeUX &);

	virtual SnakeUX &operator=(SnakeUX const &);

	bool isAlive;
	bool isSwitchingLibrary;
};

class SnakeUN {
public:
	SnakeUN();

	virtual SnakeUN &operator=(SnakeUN &);

	virtual SnakeUN &operator=(SnakeUN const &);

	friend std::ostream &operator<<(std::ostream &os, const SnakeUN &un);

	bool isUpdate;
	eDirection direction;
};

class Snake : public SnakeUI, public SnakeUX, public SnakeUN {
public:
	Snake();

	void randomSnake(int16_t id_);

	Snake &operator=(Snake &);

	Snake &operator=(Snake const &);

	Snake &operator=(SnakeUI &ui);

	Snake &operator=(SnakeUI const &ui);

	friend std::ostream &operator<<(std::ostream &os, const Snake &snake);

	Snake &operator=(SnakeUX &ux);

	Snake &operator=(SnakeUX const &ux);

	Snake &operator=(SnakeUN &un);

	Snake &operator=(SnakeUN const &un);

	bool isReady;
	bool isIA;
	bool isValid;
	int16_t id;
	size_t indexConnection;
	uint16_t score_;

	static std::string const basicName[SNAKE_MAX_NAME];
};


#endif //PROJECT_SNAKE_HPP
