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
		serverTCP_(nullptr),
		snakeArray_(std::make_shared<SnakeArrayContainer >())
		{
}
//void SnakeServer::callbackSnakeUN(const Snake &snakeUN) {
//	log_warn("%s", __PRETTY_FUNCTION__);
//	{
//		std::lock_guard<std::mutex> guard(mutex_);
//		(*snakeArray_)[snakeUN.id] = static_cast<SnakeUN>(snakeUN);
//		serverTCP_->writeDataToOpenConnections(snakeUN, eHeader::kSnakeUN);
//	}
//	updateInput();
//}

inline void SnakeServer::updateInput() {
	std::lock_guard<std::mutex> guard(mutex_);

	log_warn("Condition [%d][%d]",pause_ ,std::any_of((*snakeArray_).begin(), (*snakeArray_).end(),[](const Snake &snake){ return snake.isValid && snake.isAlive && !snake.isUpdate;} ));

	if (pause_ || std::any_of((*snakeArray_).begin(), (*snakeArray_).end(), [](const Snake & snake){ return snake.isValid && snake.isAlive && !snake.isUpdate; } )) return ;

	std::for_each((*snakeArray_).begin(), (*snakeArray_).end(), [](Snake &snake){ snake.isUpdate = false; });

	for (auto infoArray : foodInfoArray) {
		serverTCP_->writeDataToOpenConnections(infoArray, eHeader::kFood);
	}
	foodInfoArray.clear();
	serverTCP_->writeDataToOpenConnections('K', eHeader::kPock);
	return ;
}

void SnakeServer::startGame() {
	assert(isReady());
	std::lock_guard<std::mutex> guard(mutex_);
	foodInfoArray.clear();

	std::for_each((*snakeArray_).begin(), (*snakeArray_).end(), [](Snake &snake){ snake.isAlive = true; snake.direction = kNorth; });
	serverTCP_->writeDataToOpenConnections((*snakeArray_), eHeader::kSnakeArray);

	serverTCP_->writeDataToOpenConnections<StartInfo>({
		serverTCP_->getSizeOfOpenConnection(),
		boost::posix_time::microsec_clock::universal_time() + boost::posix_time::seconds(2)
		}, eHeader::kStartGame);
}

bool SnakeServer::isReady() const {
	return std::none_of((*snakeArray_).begin(), (*snakeArray_).end(), [](Snake const &snake){ return snake.isValid && !snake.isReady; });
}

bool SnakeServer::isFull() const {
	return serverTCP_->getSizeOfOpenConnection() == SNAKE_MAX;
}

unsigned short SnakeServer::getPort_() const {
	return port_;
}

void SnakeServer::notifyBorderless() {
	serverTCP_->writeDataToOpenConnections(univers_.isBorderless(), eHeader::kBorderless);
}


void SnakeServer::notifyMapSize() {
	serverTCP_->writeDataToOpenConnections(univers_.getMapSize(), eHeader::kResizeMap);
}

std::shared_ptr<SnakeArrayContainer> SnakeServer::getSnakeArray_() const {
	return snakeArray_;
}

bool SnakeServer::allSnakeIsDead() const {
	return std::all_of((*snakeArray_).begin(), (*snakeArray_).end(), [](Snake const &snake){ return snake.isAlive; });
}

bool SnakeServer::allSnakeIsReady() const {
	return std::all_of((*snakeArray_).begin(), (*snakeArray_).end(), [](Snake const &snake){ return snake.isReady; });
}

bool SnakeServer::sendOpenGameToClient() {
	log_success("%s %d", __PRETTY_FUNCTION__, isReady());
	if (!isReady())
		return false;
	serverTCP_->writeDataToOpenConnections(true, eHeader::kOpenGame);
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

bool SnakeServer::isOpen() const {
	return serverTCP_ != nullptr && serverTCP_->isOpen();
}


SnakeServer::~SnakeServer() {
	serverTCP_->writeDataToOpenConnections('*', eHeader::kCloseConnection);
}

/** Callback **/


void SnakeServer::callbackSnakeUI(const Snake &snakeUI) {
	log_success("%s %d", __PRETTY_FUNCTION__, snakeUI.isReadyToExpose);
	std::lock_guard<std::mutex> guard(mutex_);
	(*snakeArray_)[snakeUI.id] = static_cast<SnakeUI>(snakeUI);
	serverTCP_->writeDataToOpenConnections(snakeUI, eHeader::kSnakeUI);
}

void SnakeServer::callbackSnakeUX(const Snake &snakeUX) {
	std::lock_guard<std::mutex> guard(mutex_);
	(*snakeArray_)[snakeUX.id] = static_cast<SnakeUX>(snakeUX);
	serverTCP_->writeDataToOpenConnections(snakeUX, eHeader::kSnakeUX);
}

void SnakeServer::callbackSnake(const Snake &snake) {
	std::lock_guard<std::mutex> guard(mutex_);
	(*snakeArray_)[snake.id] = snake;
	serverTCP_->writeDataToOpenConnections((*snakeArray_)[snake.id], eHeader::kSnake);
}

void SnakeServer::callbackSnakeUN(const Snake &snakeUN) {
	{
		std::lock_guard<std::mutex> guard(mutex_);
		(*snakeArray_)[snakeUN.id] = static_cast<SnakeUN>(snakeUN);
		serverTCP_->writeDataToOpenConnections(snakeUN, eHeader::kSnakeUN);
	}
	updateInput();
}

void SnakeServer::callbackBorderless(bool borderless) {
	std::lock_guard<std::mutex> guard(mutex_);
	serverTCP_->writeDataToOpenConnections(borderless, eHeader::kBorderless);
}


void SnakeServer::callbackResizeMap(unsigned int mapSize) {
	std::lock_guard<std::mutex> guard(mutex_);
	serverTCP_->writeDataToOpenConnections(mapSize, eHeader::kResizeMap);
}


void SnakeServer::callbackOpenGame(bool openGame) {
	std::lock_guard<std::mutex> guard(mutex_);
	serverTCP_->writeDataToOpenConnections(openGame, eHeader::kOpenGame);
}



void SnakeServer::callbackChatInfo(ChatInfo chatInfo) {
	std::lock_guard<std::mutex> guard(mutex_);
	serverTCP_->writeDataToOpenConnections(chatInfo, eHeader::kChat);
}


void SnakeServer::callbackFood(FoodInfo foodInfo) {
	std::lock_guard<std::mutex> guard(mutex_);
	foodInfoArray.push_back(foodInfo);
}


void SnakeServer::callbackForcePause(int16_t id) {
	std::lock_guard<std::mutex> guard(mutex_);
	(*snakeArray_)[id].isSwitchingLibrary = !(*snakeArray_)[id].isSwitchingLibrary;
	pause_ = true;
}

void SnakeServer::callbackDeadConnection(size_t index) {
	{
		std::lock_guard<std::mutex> guard(mutex_);
		auto *it = std::find_if((*snakeArray_).begin(), (*snakeArray_).end(),
								[index](Snake const &snake){
									return snake.indexConnection == index;
								});
		if (it != (*snakeArray_).end()) {
			log_success("%s index : %d", __PRETTY_FUNCTION__, index);
			(*it).isValid = false;
			serverTCP_->writeDataToOpenConnections(*it, eHeader::kSnake);
		}
	}
	updateInput();
}

void SnakeServer::callbackPause(eAction pause) {
	log_success("%s", __PRETTY_FUNCTION__ );
	{
		std::lock_guard<std::mutex> guard(mutex_);
		if (std::any_of((*snakeArray_).begin(), (*snakeArray_).end(), [](Snake const &snake){ return snake.isSwitchingLibrary; })) return;
		pause_ = !pause_;
	}
	if (!pause_)
		updateInput();
	serverTCP_->writeDataToOpenConnections(pause, eHeader::kPause);
}

void SnakeServer::callbackAccept(size_t index) {

	int16_t new_id = std::distance((*snakeArray_).begin(), std::find_if((*snakeArray_).begin(), (*snakeArray_).end(), [](Snake const &s){ return !s.isValid;}));
	log_success("%s isValid : %d new_id %d index %d %d", __PRETTY_FUNCTION__, std::count_if((*snakeArray_).begin(), (*snakeArray_).end(), [](Snake const &s){ return s.isValid;}), new_id, index,
				sizeof(SnakeArrayContainer));
	(*snakeArray_)[new_id].randomSnake(new_id);
	(*snakeArray_)[new_id].indexConnection = index;

	serverTCP_->writeDataToOpenConnections((*snakeArray_), eHeader::kSnakeArray);
	serverTCP_->writeDataToOpenConnection(univers_.isBorderless(), index, eHeader::kBorderless);
	serverTCP_->writeDataToOpenConnection(univers_.getMapSize(), index, eHeader::kResizeMap);
	serverTCP_->writeDataToOpenConnection(new_id, index, eHeader::kId);
}



void SnakeServer::build() {

	boost::weak_ptr<SnakeServer> thisWeakPtr(shared_from_this());

	serverTCP_ = KNW::ServerTCP::create(univers_.getIoManager());
	serverTCP_->startServer(port_);
	serverTCP_->startAsyncAccept();

	serverTCP_->startAsyncAccept(
			([thisWeakPtr](size_t index) { auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackAccept(index); }),
			([thisWeakPtr](size_t index){ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackDeadConnection(index); }));

	serverTCP_->getDataTCP().addDataType<Snake >(
			([thisWeakPtr](const Snake snakeUI)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnakeUI(snakeUI); }),
			eHeader::kSnakeUI);

	serverTCP_->getDataTCP().addDataType<Snake >(
			([thisWeakPtr](const Snake snakeUX)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnakeUX(snakeUX); }),
			eHeader::kSnakeUX);

	serverTCP_->getDataTCP().addDataType<Snake >(
			([thisWeakPtr](const Snake snake)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnake(snake); }),
			eHeader::kSnake);

	serverTCP_->getDataTCP().addDataType<Snake >(
			([thisWeakPtr](const Snake snakeUN)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnakeUN(snakeUN); }),
			eHeader::kSnakeUN);

	serverTCP_->getDataTCP().addDataType<SnakeArrayContainer>(
			nullptr, eHeader::kSnakeArray);

	serverTCP_->getDataTCP().addDataType<char>(
			nullptr, eHeader::kPock);

	serverTCP_->getDataTCP().addDataType<bool>(
			([thisWeakPtr](const bool borderless)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackBorderless(borderless); }),
			eHeader::kBorderless);

	serverTCP_->getDataTCP().addDataType<unsigned int>(
			([thisWeakPtr](const unsigned int mapSize)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackResizeMap(mapSize); }),
			eHeader::kResizeMap);

	serverTCP_->getDataTCP().addDataType<bool>(
			([thisWeakPtr](const bool openGame)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackOpenGame(openGame); }),
			eHeader::kOpenGame);

	serverTCP_->getDataTCP().addDataType<ChatInfo>(
			([thisWeakPtr](ChatInfo chatInfo)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackChatInfo(chatInfo); }),
			eHeader::kChat);

	serverTCP_->getDataTCP().addDataType<StartInfo>(
			nullptr, eHeader::kStartGame);

	serverTCP_->getDataTCP().addDataType<FoodInfo>(
			([thisWeakPtr](FoodInfo foodInfo)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackFood(foodInfo); }),
			eHeader::kFood);

	serverTCP_->getDataTCP().addDataType<int16_t>(
			([thisWeakPtr](int16_t id)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackForcePause(id); }),
			eHeader::kForcePause);

	serverTCP_->getDataTCP().addDataType<eAction >(
			([thisWeakPtr](eAction e) { auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackPause(e); }),
			eHeader::kPause);

	serverTCP_->getDataTCP().addDataType<uint16_t >(
			nullptr, eHeader::kId);

	serverTCP_->getDataTCP().addDataType<char>(
			nullptr, eHeader::kCloseConnection);
}

