#include <KNW/ServerTCP.hpp>
#include "SnakeServer.hpp"
#include <cores/Univers.hpp>

SnakeServer::SnakeServer(
		Univers &univers,
		unsigned int port
		) :
		univers_(univers),
		pause_(false),
		port_(port),
		mapSize_(MAP_DEFAULT),
		serverTCP_(nullptr),
		borderless_(false)
		{


}

void SnakeServer::build() {

	boost::weak_ptr<SnakeServer> thisWeakPtr(shared_from_this());

	serverTCP_ = KNW::ServerTCP::create(univers_.getIoManager());
	serverTCP_->startServer(port_);
	serverTCP_->startAsyncAccept();

	serverTCP_->startAsyncAccept(
			([thisWeakPtr](size_t index) { auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackAccept(index); }),
			([thisWeakPtr](size_t index){ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackDeadConnection(index); }));


	serverTCP_->getDataTCP().addDataType<int16_t >(
			([thisWeakPtr](int16_t id)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackRemoveSnake(id); }),
			eHeader::kRemoveSnake);

	serverTCP_->getDataTCP().addDataType<InputInfo>(
			([thisWeakPtr](InputInfo ii)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackInput(ii); }),
			eHeader::kInput);

	serverTCP_->getDataTCP().addDataType<std::array<Snake, SNAKE_MAX>>(
			([thisWeakPtr](std::array<Snake, SNAKE_MAX> snake_array)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnakeArray(snake_array); }),
			eHeader::kSnakeArray);

	serverTCP_->getDataTCP().addDataType<char>(
			([thisWeakPtr](char c)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackPock(c); }),
			eHeader::kPock);

	serverTCP_->getDataTCP().addDataType<bool>(
			([thisWeakPtr](bool borderless)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackBorderless(borderless); }),
			eHeader::kBorderless);

	serverTCP_->getDataTCP().addDataType<unsigned int>(
			([thisWeakPtr](unsigned int mapSize)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackResizeMap(mapSize); }),
			eHeader::kResizeMap);

	serverTCP_->getDataTCP().addDataType<bool>(
			([thisWeakPtr](bool openGame)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackOpenGame(openGame); }),
			eHeader::kOpenGame);

	serverTCP_->getDataTCP().addDataType<int16_t>(
			([thisWeakPtr](int16_t id)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackId(id); }),
			eHeader::kId);

	serverTCP_->getDataTCP().addDataType<ChatInfo>(
			([thisWeakPtr](ChatInfo chatInfo)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackChatInfo(chatInfo); }),
			eHeader::kChat);

	serverTCP_->getDataTCP().addDataType<StartInfo>(
			([thisWeakPtr](StartInfo startInfo)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackStartInfo(startInfo); }),
			eHeader::kStartGame);

	serverTCP_->getDataTCP().addDataType<FoodInfo>(
			([thisWeakPtr](FoodInfo foodInfo)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackFood(foodInfo); }),
			eHeader::kFood);

	serverTCP_->getDataTCP().addDataType<Snake>(
			([thisWeakPtr](Snake snake)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnake(snake); }),
			eHeader::kSnake);

	serverTCP_->getDataTCP().addDataType<int16_t>(
			([thisWeakPtr](int16_t id)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackForcePause(id); }),
			eHeader::kForcePause);

	serverTCP_->getDataTCP().addDataType<eAction >(
			([thisWeakPtr](eAction e) { auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackPause(e); }),
			eHeader::kPause);

}

/***** Callback *****/


void SnakeServer::callbackRemoveSnake(int16_t id) {
	snake_array_[id].isAlive = false;
	serverTCP_->writeDataToOpenConnections(snake_array_[id], eHeader::kSnake);
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
		serverTCP_->writeDataToOpenConnections(*it, eHeader::kSnake);
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
	serverTCP_->writeDataToOpenConnections(pause, eHeader::kPause);
}

void SnakeServer::callbackSnake(Snake snake) {
	log_success("%s snakeid %d isready %d isIA %d", __PRETTY_FUNCTION__, snake.id, snake.isIA);
	assert(snake.id >= 0 && snake.id < SNAKE_MAX);
	snake_array_[snake.id] = snake;
	serverTCP_->writeDataToOpenConnections(snake, eHeader::kSnake);
}

void SnakeServer::callbackForcePause(int16_t id) {
	log_success("%s", __PRETTY_FUNCTION__ );
	std::lock_guard<std::mutex> guard(mutex_);
	assert(id < SNAKE_MAX);
	snake_array_[id].isSwitchingLibrary = !snake_array_[id].isSwitchingLibrary;
	pause_ = true;
	serverTCP_->writeDataToOpenConnections(snake_array_[id], eHeader::kSnake);
}

void SnakeServer::callbackAccept(size_t index) {

	int16_t new_id = std::distance(snake_array_.begin(), std::find_if(snake_array_.begin(), snake_array_.end(), [](Snake const &s){ return !s.isValid;}));
	log_success("%s isValid : %d new_id %d index %d", __PRETTY_FUNCTION__, std::count_if(snake_array_.begin(), snake_array_.end(), [](Snake const &s){ return s.isValid;}), new_id, index);
	snake_array_[new_id] = Snake::randomSnake(new_id); //TODO CHECK RACE CONDITION
	snake_array_[new_id].indexConnection = index;
	serverTCP_->writeDataToOpenConnections(snake_array_, eHeader::kSnakeArray);
	serverTCP_->writeDataToOpenConnection(borderless_, index, eHeader::kBorderless);
	serverTCP_->writeDataToOpenConnection(mapSize_, index,  eHeader::kResizeMap);
	serverTCP_->writeDataToOpenConnection(new_id, index, eHeader::kId);
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
	borderless_ = borderless;
	serverTCP_->writeDataToOpenConnections(borderless_, eHeader::kBorderless);
}

void SnakeServer::callbackResizeMap(unsigned int mapSize) {
	log_success("%s", __PRETTY_FUNCTION__ );
	mapSize_ = mapSize;
	serverTCP_->writeDataToOpenConnections(mapSize_, eHeader::kResizeMap);
}

void SnakeServer::callbackOpenGame(bool openGame) {
	log_success("%s", __PRETTY_FUNCTION__ );
	serverTCP_->writeDataToOpenConnections(openGame, eHeader::kOpenGame);
}

void SnakeServer::callbackId(int16_t) {
	assert(false);
}

void SnakeServer::callbackChatInfo(ChatInfo chatInfo) {
	serverTCP_->writeDataToOpenConnections(chatInfo, eHeader::kChat);
}

void SnakeServer::callbackSnakeArray(std::array<Snake, SNAKE_MAX>) {
	assert(false);
}


void SnakeServer::startGame() {
	assert(isReady());
	std::lock_guard<std::mutex> guard(mutex_);
	foodInfoArray.clear();
	std::for_each(snake_array_.begin(), snake_array_.end(),
				  [](auto &snake){ snake.isAlive = true; snake.isUpdate = false; });
	serverTCP_->writeDataToOpenConnections(snake_array_, eHeader::kSnakeArray);
	serverTCP_->writeDataToOpenConnections<StartInfo>({
		serverTCP_->getSizeOfOpenConnection(),
		boost::posix_time::microsec_clock::universal_time()
		}, eHeader::kStartGame);
}
void SnakeServer::updateInput() {
	for (auto &snake : snake_array_) {
		log_warn("Snake ID : [%d] isValid [%d] isAlive [%d] isUpdate [%d]", snake.id, snake.isValid, snake.isAlive, snake.isUpdate);
	}
	log_warn("Condition [%d][%d]",pause_ ,std::any_of(snake_array_.begin(), snake_array_.end(),
													  [](auto snake){ return snake.isValid && snake.isAlive && !snake.isUpdate;} ));
	if (pause_ || std::any_of(snake_array_.begin(), snake_array_.end(),
							  [](auto snake){ return snake.isValid && snake.isAlive && !snake.isUpdate;} )) return;
	std::lock_guard<std::mutex> guard(mutex_);
	serverTCP_->writeDataToOpenConnections(snake_array_, eHeader::kSnakeArray);
	for (auto infoArray : foodInfoArray) {
		serverTCP_->writeDataToOpenConnections(infoArray, eHeader::kFood);
	}
	foodInfoArray.clear();
	serverTCP_->writeDataToOpenConnections('c', eHeader::kPock);
	std::for_each(snake_array_.begin(), snake_array_.end(), [](auto &snake){ snake.isUpdate = false;});

}

bool SnakeServer::isReady() const {
	return std::none_of(snake_array_.begin(), snake_array_.end(), [](auto snake){ return snake.isValid && !snake.isReady; });
}

bool SnakeServer::isFull() const {
	return serverTCP_->getSizeOfOpenConnection() == SNAKE_MAX;
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
	serverTCP_->writeDataToOpenConnections(true, eHeader::kOpenGame);
	univers_.setOpenGame_(true);
	return true;
}

boost::shared_ptr<SnakeServer>
SnakeServer::create(Univers &univers, unsigned int port) {
	auto ptr = boost::shared_ptr<SnakeServer>(new SnakeServer(univers, port));
	ptr->build();
	return ptr;
}

void SnakeServer::closeAcceptorServer() {
	serverTCP_->stopAccept();
}


