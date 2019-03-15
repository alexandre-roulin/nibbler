#include <KNetwork/ServerTCP.hpp>
#include "SnakeServer.hpp"
#include <cores/Univers.hpp>
#include <cores/GameManager.hpp>

SnakeServer::SnakeServer(
		Univers &univers
		) :
		univers_(univers),
		pause_(false),
		serverTCP_(nullptr),
		snakeArray_(std::make_shared<SnakeArrayContainer >())
		{
}

void SnakeServer::updateInput() {
	std::lock_guard<std::mutex> guard(mutex_);
	unsigned int deltaTime = 10;
	for (const auto &s : *snakeArray_) {
		std::cout << "ID: "<< s.id << " - Update : " << s.isUpdate << " - IsInGame : " << s.isInGame << std::endl;


	}
	std::cout << __PRETTY_FUNCTION__ << "Conditions :> "<< pause_ << " : " << std::any_of((*snakeArray_).begin(), (*snakeArray_).end(), [](const Snake & snake){ return snake.isValid && snake.isInGame && !snake.isUpdate; } ) << std::endl;

	if (pause_ || std::any_of((*snakeArray_).begin(), (*snakeArray_).end(), [](const Snake & snake){ return snake.isValid && snake.isInGame && !snake.isUpdate; } )) return ;
	std::for_each((*snakeArray_).begin(), (*snakeArray_).end(), [](Snake &snake){
		snake.isUpdate = false;
		if (snake.isAlive)
			snake.score_ += GameManager::ScaleByFrame;
	});

	for (auto infoArray : foodInfoArray) {

		deltaTime += !infoArray.fromSnake ? GameManager::ScaleByRealFood :  GameManager::ScaleByFakeFood;

		serverTCP_->writeDataToOpenConnections(infoArray, eHeader::kFood);
	}
	std::cout << __PRETTY_FUNCTION__ << "SEND" << std::endl;

	foodInfoArray.clear();
	serverTCP_->writeDataToOpenConnections(deltaTime, eHeader::kPock);
}

void SnakeServer::startGame() {
	if (!isReady()) return;
	std::lock_guard<std::mutex> guard(mutex_);
	foodInfoArray.clear();

	std::for_each((*snakeArray_).begin(), (*snakeArray_).end(), [](Snake &snake){
		snake.isAlive = true;
		snake.direction = kNorth;
		snake.isInGame = snake.isValid;
		snake.isUpdate = false;
	});
	serverTCP_->writeDataToOpenConnections((*snakeArray_), eHeader::kSnakeArray);

	serverTCP_->writeDataToOpenConnections<StartInfo>({
		static_cast<size_t >(std::count_if((*snakeArray_).begin(), (*snakeArray_).end(), [](Snake &snake){ return snake.isValid; })),
		boost::posix_time::microsec_clock::universal_time() + boost::posix_time::seconds(3)
		}, eHeader::kStartGame);
}

bool SnakeServer::isReady() const {
	return std::none_of((*snakeArray_).begin(), (*snakeArray_).end(), [](Snake const &snake){ return snake.isValid && !snake.isReady; });
}

bool SnakeServer::isFull() const {
	return serverTCP_->getSizeOfOpenConnection() == SNAKE_MAX;
}

unsigned short SnakeServer::getPort_() const {
	return serverTCP_->getPort();
}


std::string const &SnakeServer::getAddress_() const {
	return serverTCP_->getAddress();
}

void SnakeServer::notifyBorderless() {
	serverTCP_->writeDataToOpenConnections(univers_.isBorderless(), eHeader::kBorderless);
}


void SnakeServer::notifyGameSpeed() {
	serverTCP_->writeDataToOpenConnections(univers_.getBaseSpeed(), eHeader::kGameSpeed);
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

bool SnakeServer::sendOpenGameToClient(bool openGame) {
	if (!isReady())
		return false;
	serverTCP_->writeDataToOpenConnections(openGame, eHeader::kOpenGame);
	return true;
}

boost::shared_ptr<SnakeServer>
SnakeServer::create(Univers &univers, const std::string dns, unsigned short port) {
	auto ptr = boost::shared_ptr<SnakeServer>(new SnakeServer(univers));
	ptr->build(dns, port);
	return ptr;
}

void SnakeServer::closeAcceptorServer() {
	if (serverTCP_)
		serverTCP_->stopAccept();

}

bool SnakeServer::isOpen() const {
	return serverTCP_ && serverTCP_->isOpen();
}


SnakeServer::~SnakeServer() {
	serverTCP_->writeDataToOpenConnections(0, 0);
}

/** Callback **/


void SnakeServer::callbackSnakeUI(const Snake &snakeUI) {
	if (snakeUI.id >= 8) return;
	{
		std::lock_guard<std::mutex> guard(mutex_);
		(*snakeArray_)[snakeUI.id] = static_cast<SnakeUI>(snakeUI);
		serverTCP_->writeDataToOpenConnections(snakeUI, eHeader::kSnakeUI);
	}
	updateInput();
}

void SnakeServer::callbackSnakeUX(const Snake &snakeUX) {
	if (snakeUX.id >= 8) return;
	{
		std::lock_guard<std::mutex> guard(mutex_);
		(*snakeArray_)[snakeUX.id] = static_cast<SnakeUX>(snakeUX);
		serverTCP_->writeDataToOpenConnections(snakeUX, eHeader::kSnakeUX);
	}
	if (!(!snakeUX.isAlive && std::any_of((*snakeArray_).begin(), (*snakeArray_).end(), [](const Snake & snake){ return snake.isValid && snake.isInGame && snake.isAlive;})) && pause_)
		pause_ = false;
	updateInput();
}

void SnakeServer::callbackSnake(const Snake &snake) {
	std::cout << __PRETTY_FUNCTION__ << "ID : " << snake.id << " - isInGame : " << snake.isInGame << std::endl;

	if (snake.id >= 8) return;
	{
		std::lock_guard<std::mutex> guard(mutex_);
		(*snakeArray_)[snake.id] = snake;
		serverTCP_->writeDataToOpenConnections((*snakeArray_)[snake.id], eHeader::kSnake);
	}
	if (!(!snake.isInGame && std::any_of((*snakeArray_).begin(), (*snakeArray_).end(), [](const Snake & snake){ return snake.isValid && snake.isInGame && snake.isAlive;})) && pause_) {
		pause_ = false;
	}
	updateInput();
}

void SnakeServer::callbackSnakeUN(const Snake &snakeUN) {
	if (snakeUN.id >= 8) return;
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
	if (mapSize > MAP_MAX || mapSize < MAP_MIN) return;
	std::lock_guard<std::mutex> guard(mutex_);
	serverTCP_->writeDataToOpenConnections(mapSize, eHeader::kResizeMap);
}


void SnakeServer::callbackOpenGame(bool openGame) {
	std::lock_guard<std::mutex> guard(mutex_);
	if (isReady())
		serverTCP_->writeDataToOpenConnections(openGame, eHeader::kOpenGame);
}



void SnakeServer::callbackChatInfo(ChatInfo chatInfo) {
	std::lock_guard<std::mutex> guard(mutex_);
	serverTCP_->writeDataToOpenConnections(chatInfo, eHeader::kChat);
}


void SnakeServer::callbackFood(FoodInfo foodInfo) {
	std::lock_guard<std::mutex> guard(mutex_);
	if (foodInfo.id_ >= 0 && foodInfo.id_ < 8) {
		std::cout << "Add Food to id : " << (*snakeArray_)[foodInfo.id_].name << " " << foodInfo.id_ << std::endl;
		(*snakeArray_)[foodInfo.id_].score_ += GameManager::ScaleByRealFood * 2;
	}
	foodInfoArray.push_back(foodInfo);
}


void SnakeServer::callbackForcePause(uint16_t id) {
	if (id >= 8 ) return;
	std::lock_guard<std::mutex> guard(mutex_);
	(*snakeArray_)[id].isSwitchingLibrary = !(*snakeArray_)[id].isSwitchingLibrary;
	pause_ = true;
}

void SnakeServer::callbackDeadConnection(size_t index) {
	if (index >= 8) return;
	{
		std::lock_guard<std::mutex> guard(mutex_);
		auto *it = std::find_if((*snakeArray_).begin(), (*snakeArray_).end(),
								[index](Snake const &snake){
									return snake.indexConnection == index && snake.isValid;
								});
		if (it != (*snakeArray_).end()) {
			(*it).isValid = false;
			serverTCP_->writeDataToOpenConnections(*it, eHeader::kSnake);
		}
	}
	updateInput();
}

void SnakeServer::callbackPause(eAction pause) {
	if (!(pause == kPause || pause == kSwitchDisplayLibrary)) return;

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
	if (index >= 8) return;
	int16_t new_id = std::distance((*snakeArray_).begin(), std::find_if((*snakeArray_).begin(), (*snakeArray_).end(), [](Snake const &s){ return !s.isValid;}));
	(*snakeArray_)[new_id].indexConnection = index;
	serverTCP_->writeDataToOpenConnections((*snakeArray_), eHeader::kSnakeArray);
	serverTCP_->writeDataToOpenConnection(univers_.isBorderless(), index, eHeader::kBorderless);
	serverTCP_->writeDataToOpenConnection(univers_.getMapSize(), index, eHeader::kResizeMap);
	serverTCP_->writeDataToOpenConnections(univers_.getBaseSpeed(), eHeader::kGameSpeed);
	serverTCP_->writeDataToOpenConnection(new_id, index, eHeader::kId);
}


void SnakeServer::callbackGameSpeed(GameManager::eSpeed speed) {
	serverTCP_->writeDataToOpenConnections(speed, eHeader::kGameSpeed);
}


void SnakeServer::build(const std::string dns, unsigned short port) {

	boost::weak_ptr<SnakeServer> thisWeakPtr(shared_from_this());

	serverTCP_ = KNW::ServerTCP::create(univers_.getIoManager());
	serverTCP_->startServer(dns, port);


	serverTCP_->registerAcceptCallback(([thisWeakPtr](size_t index) { auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackAccept(index); }));
	serverTCP_->registerDeadConnectionCallback(([thisWeakPtr](size_t index){ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackDeadConnection(index); }));
	serverTCP_->startAsyncAccept();

	serverTCP_->getDataTCP()->addDataType<Snake >(
			([thisWeakPtr](const Snake snakeUI)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnakeUI(snakeUI); }),
			eHeader::kSnakeUI);

	serverTCP_->getDataTCP()->addDataType<Snake >(
			([thisWeakPtr](const Snake snakeUX)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnakeUX(snakeUX); }),
			eHeader::kSnakeUX);

	serverTCP_->getDataTCP()->addDataType<Snake >(
			([thisWeakPtr](const Snake snake)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnake(snake); }),
			eHeader::kSnake);

	serverTCP_->getDataTCP()->addDataType<Snake >(
			([thisWeakPtr](const Snake snakeUN)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnakeUN(snakeUN); }),
			eHeader::kSnakeUN);

	serverTCP_->getDataTCP()->addDataType<SnakeArrayContainer>(
			nullptr, eHeader::kSnakeArray);

	serverTCP_->getDataTCP()->addDataType<unsigned int>(
			nullptr, eHeader::kPock);

	serverTCP_->getDataTCP()->addDataType<bool>(
			([thisWeakPtr](const bool borderless)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackBorderless(borderless); }),
			eHeader::kBorderless);

	serverTCP_->getDataTCP()->addDataType<unsigned int>(
			([thisWeakPtr](const unsigned int mapSize)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackResizeMap(mapSize); }),
			eHeader::kResizeMap);

	serverTCP_->getDataTCP()->addDataType<bool>(
			([thisWeakPtr](const bool openGame)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackOpenGame(openGame); }),
			eHeader::kOpenGame);

	serverTCP_->getDataTCP()->addDataType<ChatInfo>(
			([thisWeakPtr](ChatInfo chatInfo)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackChatInfo(chatInfo); }),
			eHeader::kChat);

	serverTCP_->getDataTCP()->addDataType<StartInfo>(
			nullptr, eHeader::kStartGame);

	serverTCP_->getDataTCP()->addDataType<FoodInfo>(
			([thisWeakPtr](FoodInfo foodInfo)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackFood(foodInfo); }),
			eHeader::kFood);

	serverTCP_->getDataTCP()->addDataType<uint16_t >(
			([thisWeakPtr](uint16_t  id)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackForcePause(id); }),
			eHeader::kForcePause);

	serverTCP_->getDataTCP()->addDataType<eAction >(
			([thisWeakPtr](eAction e) { auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackPause(e); }),
			eHeader::kPause);

	serverTCP_->getDataTCP()->addDataType<uint16_t >(
			nullptr, eHeader::kId);

	serverTCP_->getDataTCP()->addDataType<char>(
			nullptr, eHeader::kCloseConnection);

	serverTCP_->getDataTCP()->addDataType<GameManager::eSpeed >(
			([thisWeakPtr](GameManager::eSpeed speed) { auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackGameSpeed(speed); }),
			eHeader::kGameSpeed);

}

void SnakeServer::showScore() {

	const char position[4][3]{
			"st",
			"nd",
			"rd",
			"th"
	};

	std::vector<Snake> vector(8);

	size_t n = 0;
	std::for_each(vector.begin(), vector.end(), [&n, this](Snake &snake){ snake.deepCopy((*snakeArray_)[n++]);});
	std::sort(vector.begin(), vector.end(), [](Snake const &lhs, Snake const &rhs) {
		return lhs.score_ > rhs.score_;
	});
	n = 0;
	std::for_each(vector.begin(), vector.end(), [this, &n, &position](Snake const & snake){
		if (snake.isValid && snake.isInGame) {
			std::string s;
			s = "is in ";
			s += static_cast<char>(n + 49);
			s += position[(n > 3 ? 3 : n)];
			s += " position with ";
			s += std::to_string(snake.score_);
			s += " score.";
			n++;
			serverTCP_->writeDataToOpenConnections(ChatInfo(snake.name, s.c_str()), eHeader::kChat);
		}
	});
	std::for_each(snakeArray_->begin(), snakeArray_->end(), [](Snake &snake){ snake.score_ = 0; });
}
