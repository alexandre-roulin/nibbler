#ifndef PROJECT_SNAKE_HPP
#define PROJECT_SNAKE_HPP

#include <cstdio>
#include <Sprite.hpp>
#include <nibbler.hpp>

class BaseSnake {
public:
	BaseSnake();
	~BaseSnake() = default;
	BaseSnake(const BaseSnake &) = default;
	bool isReady;
	bool isIA;
	bool isValid;
	int16_t id;
	size_t indexConnection;
	uint16_t score_;

};

class SnakeUI : virtual public BaseSnake {
public:
	SnakeUI();
	SnakeUI &operator=(SnakeUI const &SnakeUI)  = default;
	virtual ~SnakeUI() = default;

	SnakeUI(Snake &);
	SnakeUI(const Snake &);
	virtual SnakeUI &operator=(Snake &SnakeUI);
	virtual SnakeUI &operator=(Snake const &SnakeUI);

	SnakeUI(const SnakeUI &) = default;
	char name[NAME_BUFFER];
	eSprite sprite;
	bool isReadyToExpose;

};
class SnakeUX : virtual public BaseSnake {
public:
	SnakeUX &operator=(SnakeUX const &SnakeUX) = default;
	SnakeUX();
	virtual ~SnakeUX() = default;
	SnakeUX(const SnakeUX &) = default;
	bool isUpdate;
	eDirection direction;
	bool isAlive;
	bool isSwitchingLibrary;
};

class Snake : virtual public SnakeUI, virtual public SnakeUX {
public:
	Snake();

	virtual ~Snake() = default;

	Snake(Snake &) = default;
	Snake(const Snake &) = default;
	virtual Snake &operator=(Snake &Snake) = default;
	virtual Snake &operator=(Snake const &Snake) = default;


	void
	randomSnake(int16_t id_);

	static std::string const basicName[SNAKE_MAX_NAME];
};


#endif //PROJECT_SNAKE_HPP
