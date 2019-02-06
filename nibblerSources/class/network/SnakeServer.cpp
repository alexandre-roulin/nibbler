#include "SnakeServer.hpp"
#include <Univers.hpp>

SnakeServer::SnakeServer(
		Univers &univers,
		unsigned int port
		) :
		univers_(univers),
		pause_(false),
		port_(port),
		mapSize_(MAP_DEFAULT),
		serverTCP_(port,
				std::bind(&SnakeServer::callbackDeadConnection, this, std::placeholders::_1)
				)
		{

	serverTCP_.addDataType<int16_t >(
			std::bind(&SnakeServer::callbackRemoveSnake, this, std::placeholders::_1),
			eHeaderK::kRemoveSnake);

	serverTCP_.addDataType<InputInfo>(
			std::bind(&SnakeServer::callbackInput, this, std::placeholders::_1),
			eHeaderK::kInput);

	serverTCP_.addDataType<std::array<Snake, SNAKE_MAX>>(
			std::bind(&SnakeServer::callbackSnakeArray,
					  this, std::placeholders::_1) ,eHeaderK::kSnakeArray);

	serverTCP_.addDataType<char>(
			std::bind(&SnakeServer::callbackPock,
					  this, std::placeholders::_1) ,eHeaderK::kPock);

	serverTCP_.addDataType<bool>(
			std::bind(&SnakeServer::callbackBorderless,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kBorderless);

	serverTCP_.addDataType<unsigned int>(
			std::bind(&SnakeServer::callbackResizeMap,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kResizeMap);

	serverTCP_.addDataType<bool>(
			std::bind(&SnakeServer::callbackOpenGame,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kOpenGame);

	serverTCP_.addDataType<int16_t>(
			std::bind(&SnakeServer::callbackId,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kId);

	serverTCP_.addDataType<ChatInfo>(
			std::bind(&SnakeServer::callbackChatInfo,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kChat);

	serverTCP_.addDataType<StartInfo>(
			std::bind(&SnakeServer::callbackStartInfo,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kStartGame);

	serverTCP_.addDataType<FoodInfo>(
			std::bind(&SnakeServer::callbackFood,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kFood);

	serverTCP_.addDataType<Snake>(
			std::bind(&SnakeServer::callbackSnake,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kSnake);

	serverTCP_.addDataType<int16_t>(
			std::bind(&SnakeServer::callbackForcePause,
					  this,
					  std::placeholders::_1),
			eHeaderK::kForcePause);

	serverTCP_.addDataType<eAction >(
			std::bind(&SnakeServer::callbackPause,
					  this,
					  std::placeholders::_1),
			eHeaderK::kPause);

	serverTCP_.accept(
			std::bind(&SnakeServer::callbackAccept,
					this,
					std::placeholders::_1));

}

/***** Callback *****/


void SnakeServer::callbackRemoveSnake(int16_t id) {
	snake_array_[id].isAlive = false;
	serverTCP_.writeDataToOpenConnections(snake_array_[id], eHeaderK::kSnake);
	updateInput();
}

void SnakeServer::callbackDeadConnection(size_t index) {
	auto *it = std::find_if(snake_array_.begin(), snake_array_.end(),
			[index](Snake const &snake){
				return snake.indexConnection == index;
	});

	if (it != snake_array_.end()) {
		log_success("%s index : %d", __PRETTY_FUNCTION__, index);
		(*it).isValid = false;
		serverTCP_.writeDataToOpenConnections(*it, eHeaderK::kSnake);
		updateInput();
	}
}

void SnakeServer::callbackStartInfo(StartInfo) {
	log_success("%s", __PRETTY_FUNCTION__ );

}

void SnakeServer::callbackFood(FoodInfo foodInfo) {
	log_success("%s", __PRETTY_FUNCTION__ );
	mutex_.lock();
	foodInfoArray.push_back(foodInfo);
	mutex_.unlock();
}

void SnakeServer::callbackPause(eAction pause) {
	log_success("%s", __PRETTY_FUNCTION__ );
	if (std::any_of(snake_array_.begin(), snake_array_.end(),
					[](auto snake){ return snake.isSwitchingLibrary; })) {
		return;
	}
	mutex_.lock();
	pause_ = !pause_;
	mutex_.unlock();
	if (!pause_)
		updateInput();
	serverTCP_.writeDataToOpenConnections(pause, eHeaderK::kPause);
}

void SnakeServer::callbackSnake(Snake snake) {
	log_success("%s", __PRETTY_FUNCTION__ );
	assert(snake.id >= 0 && snake.id < SNAKE_MAX);
	snake_array_[snake.id] = snake;
	serverTCP_.writeDataToOpenConnections(snake, eHeaderK::kSnake);
}

void SnakeServer::callbackForcePause(int16_t id) {
	log_success("%s", __PRETTY_FUNCTION__ );
	mutex_.lock();
	assert(id < SNAKE_MAX);
	snake_array_[id].isSwitchingLibrary = !snake_array_[id].isSwitchingLibrary;
	pause_ = std::any_of(snake_array_.begin(), snake_array_.end(), [](auto snake){
		return snake.isValid && snake.isSwitchingLibrary;
	});
	serverTCP_.writeDataToOpenConnections(snake_array_[id], eHeaderK::kSnake);

	mutex_.unlock();
	if (!pause_) {
		serverTCP_.writeDataToOpenConnections(eAction::kPause, eHeaderK::kPause);
		updateInput();
	}
}

void SnakeServer::callbackAccept(size_t index) {

	int16_t new_id = std::distance(snake_array_.begin(), std::find_if(snake_array_.begin(), snake_array_.end(), [](Snake const &s){ return !s.isValid;}));
	log_success("%s isValid : %d new_id %d index %d", __PRETTY_FUNCTION__, std::count_if(snake_array_.begin(), snake_array_.end(), [](Snake const &s){ return s.isValid;}), new_id, index);
	snake_array_[new_id] = Snake::randomSnake(new_id); //TODO CHECK RACE CONDITION
	snake_array_[new_id].indexConnection = index;
	serverTCP_.writeDataToOpenConnection(new_id, index, eHeaderK::kId);
	serverTCP_.writeDataToOpenConnections(snake_array_, eHeaderK::kSnakeArray);
}

void SnakeServer::callbackInput(InputInfo inputInfo) {
	log_success("%s ID : %d", __PRETTY_FUNCTION__, inputInfo.id_);
	mutex_.lock();
	snake_array_[inputInfo.id_].direction = inputInfo.dir_;
	snake_array_[inputInfo.id_].isUpdate = true;
	mutex_.unlock();
	updateInput();
}

void SnakeServer::callbackPock(char) {
	log_success("%s", __PRETTY_FUNCTION__);

}

void SnakeServer::callbackBorderless(bool borderless) {
	log_success("%s", __PRETTY_FUNCTION__ );
	serverTCP_.writeDataToOpenConnections(borderless, eHeaderK::kBorderless);
}

void SnakeServer::callbackResizeMap(unsigned int mapSize) {
	log_success("%s", __PRETTY_FUNCTION__ );
	mapSize_ = mapSize;
	serverTCP_.writeDataToOpenConnections(mapSize, eHeaderK::kResizeMap);
}

void SnakeServer::callbackOpenGame(bool openGame) {
	log_success("%s", __PRETTY_FUNCTION__ );
	serverTCP_.writeDataToOpenConnections(openGame, eHeaderK::kOpenGame);

}

void SnakeServer::callbackId(int16_t) {
	log_success("%s", __PRETTY_FUNCTION__ );
}

void SnakeServer::callbackChatInfo(ChatInfo chatInfo) {
	log_success("%s", __PRETTY_FUNCTION__ );
	serverTCP_.writeDataToOpenConnections(chatInfo, eHeaderK::kChat);
}

void SnakeServer::callbackSnakeArray(std::array<Snake, SNAKE_MAX> array) {
	log_error("%s ARRAY !?!?!?!?!?", __PRETTY_FUNCTION__ );
	snake_array_ = array;
}


void SnakeServer::startGame() {
	assert(isReady());
	mutex_.lock();
	foodInfoArray.clear();
	StartInfo startInfo;
	std::for_each(snake_array_.begin(), snake_array_.end(),
				  [](auto &snake){ snake.isAlive = true; snake.isUpdate = false; });
	serverTCP_.writeDataToOpenConnections(snake_array_, eHeaderK::kSnakeArray);
	startInfo.nu = serverTCP_.getSizeOfConnections();
	startInfo.time_duration = boost::posix_time::microsec_clock::universal_time();
	serverTCP_.writeDataToOpenConnections(startInfo, eHeaderK::kStartGame);
	mutex_.unlock();
}
void SnakeServer::updateInput() {
	log_warn("Condition [%d][%d]",pause_ ,std::any_of(snake_array_.begin(), snake_array_.end(),
													  [](auto snake){ return snake.isValid && snake.isAlive && !snake.isUpdate;} ));
	if (pause_ || std::any_of(snake_array_.begin(), snake_array_.end(),
							  [](auto snake){ return snake.isValid && snake.isAlive && !snake.isUpdate;} )) return;
	mutex_.lock();
	serverTCP_.writeDataToOpenConnections(snake_array_, eHeaderK::kSnakeArray);
	for (auto infoArray : foodInfoArray) {
		serverTCP_.writeDataToOpenConnections(infoArray, eHeaderK::kFood);
	}
	foodInfoArray.clear();
	serverTCP_.writeDataToOpenConnections('c', eHeaderK::kPock);
	std::for_each(snake_array_.begin(), snake_array_.end(), [](auto &snake){ snake.isUpdate = false;});
	mutex_.unlock();

	log_info("How many null %d",std::count_if(serverTCP_.connections.begin(), serverTCP_.connections.end(),[](auto ptr){
			return ptr != nullptr; }));
}

bool SnakeServer::isReady() const {
	return std::none_of(snake_array_.begin(), snake_array_.end(), [](auto snake){ return snake.isValid && !snake.isReady; });
}

bool SnakeServer::isFull() const {
	return serverTCP_.getSizeOfConnections() == SNAKE_MAX;
}

unsigned short SnakeServer::getPort_() const {
	return port_;
}

const std::array<Snake, SNAKE_MAX> &SnakeServer::getSnakeArray_() const {
	return snake_array_;
}

bool SnakeServer::sendOpenGameToClient() {
	log_success("%s %d", __PRETTY_FUNCTION__, isReady());
	if (!isReady())
		return false;
	serverTCP_.writeDataToOpenConnections(true, eHeaderK::kOpenGame);
	univers_.setOpenGame_(true);
	return true;
}
