#include "Snake.hpp"

std::string const Snake::basicName[SNAKE_MAX_NAME] = {
		"Jack O'Lantern",
		"Eden",
		"Jacky",
		"Emerald",
		"Broutille",
		"Veggie-vie",
		"jinou42",
		"Dautta c bo"
};

Snake::Snake() :
		sprite(eSprite::kBlue),
		isReady(false),
		id(-1),
		isUpdate(false),
		direction(kNorth),
		isAlive(false),
		isSwitchingLibrary(false),
		isValid(false) {
	memset(name, 0, NAME_BUFFER);
}

Snake &Snake::operator=(Snake const &snake) {
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
		isValid = snake.isValid;
	}
	return *this;
}

Snake Snake::randomSnake(int16_t id) {
	Snake snake;

	snake.sprite = static_cast<eSprite>(static_cast<int>(eSprite::kGreen) + rand() % SNAKE_MAX_COLOR);
	strncpy(snake.name, Snake::basicName[rand() % SNAKE_MAX_NAME].c_str(), NAME_BUFFER);
	snake.id = id;
	snake.isAlive = true;
	snake.isSwitchingLibrary = false;
	snake.isValid = true;
	return (snake);
};
