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

Snake::Snake()  {}

SnakeUI::SnakeUI() :
		sprite(eSprite::kBlue),
		isReadyToExpose(false) {
	memset(name, 0, NAME_BUFFER);
}

std::ostream &operator<<(std::ostream &os, const SnakeUI &ui) {
	os << static_cast<const BaseSnake &>(ui) << " name: " << ui.name
	   << " sprite: "  << " isReadyToExpose: "
	   << ui.isReadyToExpose;
	return os;
}

SnakeUX::SnakeUX() :
		isUpdate(false),
		direction(kNorth),
		isAlive(false),
		isSwitchingLibrary(false) {
}

void SnakeUX::reset() {
	if (isValid) {
		isUpdate = false;
		direction = kNorth;
		isAlive = true;
		isSwitchingLibrary = false;
	}
}

void SnakeUX::updateDirection(eDirection dir) {
	direction = dir;
	isUpdate = true;
}

std::ostream &operator<<(std::ostream &os, const SnakeUX &ux) {
	os << static_cast<const BaseSnake &>(ux) << " isUpdate: " << ux.isUpdate
	   << " direction: " << ux.direction << " isAlive: " << ux.isAlive
	   << " isSwitchingLibrary: " << ux.isSwitchingLibrary;
	return os;
}

BaseSnake::BaseSnake() :
		isReady(false),
		isIA(false),
		isValid(false),
		id(-1),
		indexConnection(0),
		score_(0)
		{

}

std::ostream &operator<<(std::ostream &os, const BaseSnake &snake) {
	os << "isReady: " << snake.isReady << " isIA: " << snake.isIA
	   << " isValid: " << snake.isValid << " id: " << snake.id
	   << " indexConnection: " << snake.indexConnection << " score_: "
	   << snake.score_;
	return os;
}

void Snake::randomSnake(int16_t id_) {

	sprite = static_cast<eSprite>(static_cast<int>(eSprite::kGreen) + rand() % SNAKE_MAX_COLOR);
	strncpy(name, Snake::basicName[rand() % SNAKE_MAX_NAME].c_str(), NAME_BUFFER);
	id = id_;
	isAlive = true;
	isSwitchingLibrary = false;
	isValid = true;
	isIA = false;
	score_ = 0;
	isReadyToExpose = false;
}


Snake &Snake::operator=(Snake &snake) {
	isReady = snake.isReady;
	isIA = snake.isIA;
	isValid = snake.isValid;
	id = snake.id;
	indexConnection = snake.indexConnection;
	score_ = snake.score_;
	std::strncpy(name, snake.name, NAME_BUFFER);
	sprite  = snake.sprite;
	isReadyToExpose  = snake.isReadyToExpose;
	isUpdate = snake.isUpdate;
	direction = snake.direction;
	isAlive = snake.isAlive;
	return *this;
}

Snake &Snake::operator=(Snake const &snake) {
	isReady = snake.isReady;
	isIA = snake.isIA;
	isValid = snake.isValid;
	id = snake.id;
	indexConnection = snake.indexConnection;
	score_ = snake.score_;
	std::strncpy(name, snake.name, NAME_BUFFER);
	sprite  = snake.sprite;
	isReadyToExpose  = snake.isReadyToExpose;
	isUpdate = snake.isUpdate;
	direction = snake.direction;
	isAlive = snake.isAlive;
	return *this;
}

Snake &Snake::operator=(SnakeUX &snakeUX) noexcept {
	isUpdate = snakeUX.isUpdate;
	direction = snakeUX.direction;
	isAlive = snakeUX.isAlive;
	return *this;
}

Snake &Snake::operator=(SnakeUX const &snakeUX) noexcept {
	isUpdate = snakeUX.isUpdate;
	direction = snakeUX.direction;
	isAlive = snakeUX.isAlive;
	return *this;
}

Snake &Snake::operator=(SnakeUI &snakeUI) noexcept {
	std::strncpy(name, snakeUI.name, NAME_BUFFER);
	sprite  = snakeUI.sprite;
	isReadyToExpose  = snakeUI.isReadyToExpose;
	return *this;
}

Snake &Snake::operator=(SnakeUI const &snakeUI) noexcept {
	std::strncpy(name, snakeUI.name, NAME_BUFFER);
	sprite  = snakeUI.sprite;
	isReadyToExpose  = snakeUI.isReadyToExpose;
	return *this;
}

Snake &Snake::operator=(BaseSnake &baseSnake) noexcept {
	isReady = baseSnake.isReady;
	isIA = baseSnake.isIA;
	isValid = baseSnake.isValid;
	id = baseSnake.id;
	indexConnection = baseSnake.indexConnection;
	score_ = baseSnake.score_;
	return *this;
}

Snake &Snake::operator=(BaseSnake const &baseSnake) noexcept {
	isReady = baseSnake.isReady;
	isIA = baseSnake.isIA;
	isValid = baseSnake.isValid;
	id = baseSnake.id;
	indexConnection = baseSnake.indexConnection;
	score_ = baseSnake.score_;
	return *this;
}

std::ostream &operator<<(std::ostream &os, const Snake &snake) {
	os << static_cast<const SnakeUI &>(snake) << ' '
	   << static_cast<const BaseSnake &>(snake) << ' '
		<<static_cast<const SnakeUX &>(snake);
	return os;
}
