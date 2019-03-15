#ifndef PROJECT_SNAKE_HPP
#define PROJECT_SNAKE_HPP

#include <cstdio>
#include <Sprite.hpp>
#include <nibbler.hpp>
#include <ostream>

class SnakeUI {
public:
	SnakeUI();
	virtual ~SnakeUI() = default;
	virtual SnakeUI &operator=(SnakeUI &);
	virtual SnakeUI &operator=(SnakeUI const &);
	SnakeUI(const SnakeUI &) = default;

	char name[NAME_BUFFER];

	friend std::ostream &operator<<(std::ostream &os, const SnakeUI &ui);

	eSprite sprite;
	bool isReadyToExpose;
	uint16_t score_;
};

class SnakeUX {
public:
	friend std::ostream &operator<<(std::ostream &os, const SnakeUX &ux);

	SnakeUX();
	virtual ~SnakeUX() = default;
	virtual SnakeUX &operator=(SnakeUX &);
	virtual SnakeUX &operator=(SnakeUX const &);
	SnakeUX(const SnakeUX &) = default;

	bool isAlive;
	bool isSwitchingLibrary;
};

class SnakeUN {
public:
	SnakeUN();
	virtual ~SnakeUN() = default;
	virtual SnakeUN &operator=(SnakeUN &);
	virtual SnakeUN &operator=(SnakeUN const &);
	SnakeUN(const SnakeUN &) = default;

	friend std::ostream &operator<<(std::ostream &os, const SnakeUN &un);

	bool isUpdate;
	eDirection direction;
};

class Snake : public SnakeUI, public SnakeUX, public SnakeUN {
public:
	Snake();
	~Snake() = default;
	Snake &operator=(Snake &);
	Snake &operator=(Snake const &);
	Snake &operator=(SnakeUI &ui);
	Snake &operator=(SnakeUI const &ui);
	Snake &operator=(SnakeUX &ux);
	Snake &operator=(SnakeUX const &ux);
	Snake &operator=(SnakeUN &un);
	Snake &operator=(SnakeUN const &un);
	Snake(Snake &) = default;
	Snake(const Snake &) = default;

	void randomSnake(int16_t id_, SnakeArrayContainer const &snakeArrayContainer);

	void deepCopy(Snake const &snake);

	friend std::ostream &operator<<(std::ostream &os, const Snake &snake);

	bool isReady;
	bool isIA;
	bool isValid;
	bool isInGame;
	uint16_t id;
	size_t indexConnection;

	static std::vector< std::string > const names;
	static std::vector< eSprite > const colors;
};

std::vector< eSprite > getUnusedColor(SnakeArrayContainer const &snakeArrayContainer);
std::vector< std::string > getUnusedName(SnakeArrayContainer const &snakeArrayContainer);


#endif //PROJECT_SNAKE_HPP
