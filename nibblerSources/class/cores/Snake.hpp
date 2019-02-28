#ifndef PROJECT_SNAKE_HPP
#define PROJECT_SNAKE_HPP

#include <cstdio>
#include <Sprite.hpp>
#include <nibbler.hpp>
#include <ostream>

class BaseSnake {
public:
	BaseSnake();

	~BaseSnake() = default;

	BaseSnake(const BaseSnake &) = default;

	virtual BaseSnake &operator=(BaseSnake &) = default;

	virtual BaseSnake &operator=(BaseSnake const &) = default;

	bool isReady;
	bool isIA;
	bool isValid;
	int16_t id;

	friend std::ostream &operator<<(std::ostream &os, const BaseSnake &snake);

	size_t indexConnection;
	uint16_t score_;

};

class SnakeUI : virtual public BaseSnake {
public:
	SnakeUI();

	virtual ~SnakeUI() = default;

	SnakeUI(const SnakeUI &) = default;

	friend std::ostream &operator<<(std::ostream &os, const SnakeUI &ui);

	virtual SnakeUI &operator=(SnakeUI &) = default;

	virtual SnakeUI &operator=(SnakeUI const &) = default;

	char name[NAME_BUFFER];
	eSprite sprite;
	bool isReadyToExpose;

};

class SnakeUX : virtual public BaseSnake {
public:
	SnakeUX();

	virtual ~SnakeUX() = default;

	SnakeUX(const SnakeUX &) = default;

	virtual SnakeUX &operator=(SnakeUX &) = default;

	virtual SnakeUX &operator=(SnakeUX const &) = default;

	void reset();

	void updateDirection(eDirection dir);

	bool isUpdate;
	eDirection direction;
	bool isAlive;

	friend std::ostream &operator<<(std::ostream &os, const SnakeUX &ux);

	bool isSwitchingLibrary;

};

class Snake : virtual public SnakeUI, virtual public SnakeUX {
public:
	Snake();

	virtual ~Snake() = default;

	Snake(Snake &) = default;

	Snake(const Snake &) = default;

	Snake &operator=(Snake &Snake);

	Snake &operator=(Snake const &Snake);

	friend std::ostream &operator<<(std::ostream &os, const Snake &snake);

	virtual Snake &operator=(SnakeUX &Snake) noexcept override;

	virtual Snake &operator=(SnakeUX const &Snake) noexcept override;

	virtual Snake &operator=(SnakeUI &Snake) noexcept override;

	virtual Snake &operator=(SnakeUI const &Snake) noexcept override;

	virtual Snake &operator=(BaseSnake &Snake) noexcept override;

	virtual Snake &operator=(BaseSnake const &Snake) noexcept override;

	void
	randomSnake(int16_t id_);

	static std::string const basicName[SNAKE_MAX_NAME];
};


#endif //PROJECT_SNAKE_HPP
