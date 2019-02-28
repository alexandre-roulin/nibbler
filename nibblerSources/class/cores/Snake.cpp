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

/** Snake **/

Snake::Snake() {

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
	if (this != &snake) {
		BaseSnake::operator=(static_cast<BaseSnake>(snake));
		SnakeUX::operator=(static_cast<SnakeUX>(snake));
		SnakeUI::operator=(static_cast<SnakeUI>(snake));
	}
	return *this;
}

Snake &Snake::operator=(Snake const &snake) {
	if (this != &snake) {
		BaseSnake::operator=(static_cast<BaseSnake>(snake));
		SnakeUX::operator=(static_cast<SnakeUX>(snake));
		SnakeUI::operator=(static_cast<SnakeUI>(snake));
	}
	return *this;
}

Snake &Snake::operator=(SnakeUX &snakeUX) noexcept {
	if (this != &snakeUX) {
		SnakeUX::operator=(snakeUX);
	}
	return *this;
}

Snake &Snake::operator=(SnakeUX const &snakeUX) noexcept {
	if (this != &snakeUX) {
		SnakeUX::operator=(snakeUX);
	}
	return *this;
}

Snake &Snake::operator=(SnakeUI &snakeUI) noexcept {
	if (this != &snakeUI) {
		SnakeUI::operator=(snakeUI);
	}
	return *this;
}

Snake &Snake::operator=(SnakeUI const &snakeUI) noexcept {
	if (this != &snakeUI) {
		SnakeUI::operator=(snakeUI);
	}
	return *this;
}

Snake &Snake::operator=(BaseSnake &baseSnake) noexcept {
	if (this != &baseSnake) {
		BaseSnake::operator=(baseSnake);
	}
	return *this;
}

Snake &Snake::operator=(BaseSnake const &baseSnake) noexcept {
	if (this != &baseSnake) {
		BaseSnake::operator=(baseSnake);
	}
	return *this;
}

std::ostream &operator<<(std::ostream &os, const Snake &snake) {
	os << static_cast<const SnakeUI &>(snake) << ' '
	   << static_cast<const BaseSnake &>(snake) << ' '
	   <<static_cast<const SnakeUX &>(snake);
	return os;
}

Snake::Snake(Snake &snake)  {
	*this = snake;
}

Snake::Snake(const Snake &snake) {
	*this = snake;
}

/** SnakeUI **/

SnakeUI::SnakeUI() :
		sprite(eSprite::kBlue),
		isReadyToExpose(false) {
	memset(name, 0, NAME_BUFFER);
}

SnakeUI::SnakeUI(SnakeUI &snake) {
	*this = snake;
}

SnakeUI::SnakeUI(const SnakeUI &snake) {
	*this = snake;
}

SnakeUI &SnakeUI::operator=(SnakeUI &snakeUI) {
	if (this != &snakeUI) {
		BaseSnake::operator=(snakeUI);
		std::strncpy(name, snakeUI.name, NAME_BUFFER);
		sprite  = snakeUI.sprite;
		isReadyToExpose  = snakeUI.isReadyToExpose;
	}
	return *this;
}

SnakeUI &SnakeUI::operator=(SnakeUI const &snakeUI) {
	if (this != &snakeUI) {
		BaseSnake::operator=(snakeUI);
		std::strncpy(name, snakeUI.name, NAME_BUFFER);
		sprite  = snakeUI.sprite;
		isReadyToExpose  = snakeUI.isReadyToExpose;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &os, const SnakeUI &ui) {
	os << static_cast<const BaseSnake &>(ui) << " name: " << ui.name
	   << " sprite: "  << " isReadyToExpose: "
	   << ui.isReadyToExpose;
	return os;
}

/** SnakeUX **/

SnakeUX::SnakeUX() :
		isUpdate(false),
		direction(kNorth),
		isAlive(false),
		isSwitchingLibrary(false) {
}

SnakeUX::SnakeUX(SnakeUX &snake) {
	*this = snake;
}

SnakeUX::SnakeUX(const SnakeUX &snake) {
	*this = snake;
}

SnakeUX &SnakeUX::operator=(SnakeUX &snakeUX) {
	if (this != &snakeUX) {
		BaseSnake::operator=(snakeUX);
		isUpdate = snakeUX.isUpdate;
		direction = snakeUX.direction;
		isAlive = snakeUX.isAlive;
		isSwitchingLibrary = snakeUX.isSwitchingLibrary;
	}
	return *this;
}


SnakeUX &SnakeUX::operator=(const SnakeUX &snakeUX) {
	if (this != &snakeUX) {
		BaseSnake::operator=(snakeUX);
		isUpdate = snakeUX.isUpdate;
		direction = snakeUX.direction;
		isAlive = snakeUX.isAlive;
		isSwitchingLibrary = snakeUX.isSwitchingLibrary;
	}
	return *this;
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


/** BaseSnake **/

BaseSnake::BaseSnake() :
		isReady(false),
		isIA(false),
		isValid(false),
		id(-1),
		indexConnection(0),
		score_(0)
		{

}

BaseSnake::BaseSnake(BaseSnake &baseSnake) {
	*this = baseSnake;
}

BaseSnake::BaseSnake(const BaseSnake &baseSnake) {
	*this = baseSnake;
}

BaseSnake &BaseSnake::operator=(BaseSnake &baseSnake) {
	if (this != &baseSnake) {
		isReady = baseSnake.isReady;
		isIA = baseSnake.isIA;
		isValid = baseSnake.isValid;
		id = baseSnake.id;
		indexConnection = baseSnake.indexConnection;
		score_ = baseSnake.score_;
	}
	return *this;
}

BaseSnake &BaseSnake::operator=(BaseSnake const &baseSnake) {
	if (this != &baseSnake) {
		isReady = baseSnake.isReady;
		isIA = baseSnake.isIA;
		isValid = baseSnake.isValid;
		id = baseSnake.id;
		indexConnection = baseSnake.indexConnection;
		score_ = baseSnake.score_;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &os, const BaseSnake &snake) {
	os << "isReady: " << snake.isReady << " isIA: " << snake.isIA
	   << " isValid: " << snake.isValid << " id: " << snake.id
	   << " indexConnection: " << snake.indexConnection << " score_: "
	   << snake.score_;
	return os;
}



