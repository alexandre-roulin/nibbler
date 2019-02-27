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
		id_(-1),
		isUpdate(false),
		direction(kNorth),
		isAlive(false),
		isIA(false),
		isSwitchingLibrary(false),
		isValid(false),
		isReadyToExpose(false),
		score_(0) {
	memset(name, 0, NAME_BUFFER);
}

void Snake::randomSnake(int16_t id) {

	sprite = static_cast<eSprite>(static_cast<int>(eSprite::kGreen) +
								  rand() % SNAKE_MAX_COLOR);
	strncpy(name, Snake::basicName[rand() % SNAKE_MAX_NAME].c_str(),
			NAME_BUFFER);
	id_ = id;
	isAlive = true;
	isSwitchingLibrary = false;
	isValid = true;
	isIA = false;
	score_ = 0;
	isReadyToExpose = false;
}

void Snake::lightCopy(Snake &snake) {
	std::strncpy(name, snake.name, NAME_BUFFER);
	score_ = snake.score_;
	id_ = snake.id_;
	sprite = snake.sprite;
	isReady = snake.isReady;
	isIA = snake.isIA;
	isReadyToExpose = snake.isReadyToExpose;
	isAlive = snake.isAlive;
}

void Snake::hardCopy(Snake &snake) {
	lightCopy(snake);
	direction = snake.direction;
	isUpdate = snake.isUpdate;
	isSwitchingLibrary = snake.isSwitchingLibrary;
	indexConnection = snake.indexConnection;
	isValid = snake.isValid;
}

void Snake::reset() {
	sprite = eSprite::kBlue;
	isReady = false;
	id_ = -1;
	isUpdate = false;
	direction = kNorth;
	isAlive = false;
	isIA = false;
	isSwitchingLibrary = false;
	isValid = false;
	score_ = 0;
	memset(name, 0, NAME_BUFFER);
};

