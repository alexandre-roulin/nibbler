#include "Snake.hpp"
#include <boost/serialization/serialization.hpp>

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

Snake::Snake() :
		isReady(false),
		isIA(false),
		isValid(false),
		id(0),
		indexConnection(0),
		score_(0) {

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
	isReadyToExpose = true;
}

Snake &Snake::operator=(SnakeUI const &ui) {
	SnakeUI::operator=(ui);
	std::cout << (isReadyToExpose  ? "true" : "false") << std::endl;
	return *this;
}

Snake &Snake::operator=(SnakeUX const &ux) {
	SnakeUX::operator=(ux);
	return *this;
}

Snake &Snake::operator=(SnakeUN const &un) {
	SnakeUN::operator=(un);
	return *this;
}

Snake &Snake::operator=(SnakeUI &ui) {
	SnakeUI::operator=(ui);
	return *this;
}

Snake &Snake::operator=(SnakeUX &ux) {
	SnakeUX::operator=(ux);
	return *this;
}

Snake &Snake::operator=(SnakeUN &un) {
	SnakeUN::operator=(un);
	return *this;
}

Snake &Snake::operator=(Snake &snake) {
	if (this != &snake) {
		isReady = snake.isReady;
		isIA = snake.isIA;
		isValid = snake.isValid;
		id = snake.id;
		indexConnection = snake.indexConnection;
		score_ = snake.score_;
	}
	return *this;
}

Snake &Snake::operator=(Snake const &snake) {
	if (this != &snake) {
		isReady = snake.isReady;
		isIA = snake.isIA;
		isValid = snake.isValid;
		id = snake.id;
		indexConnection = snake.indexConnection;
		score_ = snake.score_;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &os, const Snake &snake) {
	os << static_cast<const SnakeUI &>(snake) << ' '
	   << static_cast<const SnakeUX &>(snake) << ' '
	   << static_cast<const SnakeUN &>(snake) << " isReady: " << snake.isReady
	   << " isIA: " << snake.isIA << " isValid: " << snake.isValid << " id: "
	   << snake.id << " indexConnection: " << snake.indexConnection
	   << " score_: " << snake.score_;
	return os;
}

void Snake::deepCopy(Snake const &snake) {
	operator=(snake);
	operator=(static_cast<SnakeUI>(snake));
	operator=(static_cast<SnakeUX>(snake));
	operator=(static_cast<SnakeUN>(snake));
}


/** SnakeUI **/

SnakeUI::SnakeUI() :
		sprite(eSprite::kBlue),
		isReadyToExpose(false) {
	memset(name, 0, NAME_BUFFER);
}

SnakeUI &SnakeUI::operator=(SnakeUI &snakeUI) {
	if (this != &snakeUI) {
		isReadyToExpose = snakeUI.isReadyToExpose;
		sprite = snakeUI.sprite;
		std::memcpy(name, snakeUI.name, NAME_BUFFER);
	}
	return *this;
}

SnakeUI &SnakeUI::operator=(SnakeUI const &snakeUI) {
	if (this != &snakeUI) {
		isReadyToExpose = snakeUI.isReadyToExpose;
		sprite = snakeUI.sprite;
		std::memcpy(name, snakeUI.name, NAME_BUFFER);
	}
	return *this;
}

std::ostream &operator<<(std::ostream &os, const SnakeUI &ui) {
	os << "name: " << ui.name
	   << " isReadyToExpose: " << ui.isReadyToExpose;
	return os;
}

/** SnakeUX **/

SnakeUX::SnakeUX() :
		isAlive(false),
		isSwitchingLibrary(false) {
}

SnakeUX &SnakeUX::operator=(SnakeUX const &snakeUX) {
	if (this != &snakeUX) {
		isAlive = snakeUX.isAlive;
		isSwitchingLibrary = snakeUX.isSwitchingLibrary;
	}
	return *this;
}

SnakeUX &SnakeUX::operator=(SnakeUX &snakeUX) {
	if (this != &snakeUX) {
		isAlive = snakeUX.isAlive;
		isSwitchingLibrary = snakeUX.isSwitchingLibrary;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &os, const SnakeUX &ux) {
	os << "isAlive: " << ux.isAlive << " isSwitchingLibrary: "
	   << ux.isSwitchingLibrary;
	return os;
}

/** SnakeUN **/

SnakeUN::SnakeUN() :
		isUpdate(false),
		direction(kNorth) {

}

SnakeUN &SnakeUN::operator=(SnakeUN const &snakeUN) {
	if (this != &snakeUN) {
		isUpdate = snakeUN.isUpdate;
		direction = snakeUN.direction;
	}
	return *this;
}

SnakeUN &SnakeUN::operator=(SnakeUN &snakeUN) {
	if (this != &snakeUN) {
		isUpdate = snakeUN.isUpdate;
		direction = snakeUN.direction;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &os, const SnakeUN &un) {
	os << "isUpdate: " << un.isUpdate << " direction: " << un.direction;
	return os;
}
