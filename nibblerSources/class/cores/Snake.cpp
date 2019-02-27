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

SnakeUX::SnakeUX() :
		isUpdate(false),
		direction(kNorth),
		isAlive(false),
		isSwitchingLibrary(false) {
}

BaseSnake::BaseSnake() :
		isReady(false),
		isIA(false),
		isValid(false),
		id(-1),
		score_(0) {

}

void Snake::randomSnake(int16_t id_) {

	sprite = static_cast<eSprite>(static_cast<int>(eSprite::kGreen) +
								  rand() % SNAKE_MAX_COLOR);
	strncpy(name, Snake::basicName[rand() % SNAKE_MAX_NAME].c_str(),
			NAME_BUFFER);
	id = id_;
	isAlive = true;
	isSwitchingLibrary = false;
	isValid = true;
	isIA = false;
	score_ = 0;
	isReadyToExpose = false;
}

