#include "SnakeClient.hpp"
#include <gui/Gui.hpp>
#include <KINU/World.hpp>
#include <events/StartEvent.hpp>
#include <cores/GameManager.hpp>
#include <KNetwork/BaseDataType.hpp>
#include <KINU/World.hpp>

SnakeClient::SnakeClient(
		Univers &univers,
		bool fromIA_
		) :
		clientTCP_(nullptr),
		univers_(univers),
		fromIA_(fromIA_),
		id_(0),
		factory_(univers),
		name_(NAME_BUFFER, '\0'),
		sprite_(eSprite::kNone),
		nameSet_(false),
		spriteSet_(false),
		snakeArray(std::make_shared<SnakeArrayContainer>()){
}

SnakeClient::~SnakeClient() = default;

/***** Network Management *****/


void SnakeClient::sendDirection(eDirection direction) {
	std::lock_guard<std::mutex> guard(mutex_);
	(*snakeArray)[id_].direction = direction;
	(*snakeArray)[id_].isUpdate = true;
	std::cout << __PRETTY_FUNCTION__ << ::std::endl;
	sendDataToServer((*snakeArray)[id_], eHeader::kSnakeUN);
}

void SnakeClient::connect(std::string dns, std::string port) {
	if (clientTCP_)
		clientTCP_->connect(dns, port);
}

bool SnakeClient::acceptDataFromServer() const {
	SnakeClient::boost_shared_ptr ptr(univers_.getSnakeClient().lock());
	return !fromIA_ || (univers_.isOnlyIA() && ptr && ptr->getId_() == id_);
}

void SnakeClient::lock() {
	mutex_.lock();
}

void SnakeClient::unlock() {
	mutex_.unlock();
}

bool SnakeClient::isReady() const {
	return (*snakeArray)[id_].isReady;
}

SnakeClient::boost_shared_ptr
SnakeClient::create(Univers &univers, bool fromIA) {
	auto ptr = boost::shared_ptr<SnakeClient>(new SnakeClient(univers, fromIA));
	ptr->build();
	return ptr;
}

void SnakeClient::refreshSnakeArray() {
	if (nameSet_)
		std::memcpy((*snakeArray)[id_].name, name_.c_str(), NAME_BUFFER);
	if (spriteSet_)
		(*snakeArray)[id_].sprite = sprite_;
}

/***** Snake Management *****/


uint16_t SnakeClient::getId_() const {
	return id_;
}

bool SnakeClient::allSnakeIsDead() const {
	return std::none_of((*snakeArray).begin(), (*snakeArray).end(),
						[](Snake const &snake) {
							return snake.isValid && snake.isAlive;
						});
}

void SnakeClient::deliverEvents() {
	std::lock_guard<std::mutex> guard(mutex_);
	for (auto foodCreation : foodCreations) {
		auto world = univers_.getGameManager().getWorld_();
		if (world)
			world->getEventsManager().emitEvent(foodCreation);
	}
	foodCreations.clear();
}

bool SnakeClient::isSwitchingLibrary() const {
	return (*snakeArray)[id_].isSwitchingLibrary;
}

std::shared_ptr<SnakeArrayContainer> SnakeClient::getSnakeArray_() const {
	return snakeArray;
}

const Snake &SnakeClient::getSnake() const {
	return (*snakeArray)[id_];
}

void SnakeClient::changeName(std::string const &name) {

	nameSet_ = true;
	name_ = name;
	refreshSnakeArray();
	sendDataToServer((*snakeArray)[id_], eHeader::kSnakeUI);
}



void SnakeClient::notifyGameSpeed() {
	sendDataToServer(univers_.getBaseSpeed(), eHeader::kGameSpeed);
}

void SnakeClient::notifyBorderless() {
	sendDataToServer(univers_.isBorderless(), eHeader::kBorderless);
}

void SnakeClient::notifyMapSize() {
	sendDataToServer(univers_.getMapSize(), eHeader::kResizeMap);
}

bool SnakeClient::allSnakeIsReady() const {
	return std::all_of((*snakeArray).begin(), (*snakeArray).end(),
						[](Snake const &snake) {
							return !snake.isValid || (snake.isValid && snake.isReady);
						});;
}

void SnakeClient::quitGame() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	(*snakeArray)[id_].isInGame = false;
	sendDataToServer((*snakeArray)[id_], eHeader::kSnake);
}

void SnakeClient::changeSprite(eSprite snakeSprite) {
	spriteSet_ = true;
	sprite_ = snakeSprite;
	refreshSnakeArray();
	sendDataToServer((*snakeArray)[id_], eHeader::kSnakeUI);
}


void SnakeClient::changeStateReady(bool change) {
	(*snakeArray)[id_].isReady = change;
	sendDataToServer((*snakeArray)[id_], eHeader::kSnake);
}


bool SnakeClient::isOpen() const {
	return clientTCP_ != nullptr && clientTCP_->isConnect();
}

bool SnakeClient::isIa() const {
	return fromIA_;
}


bool SnakeClient::sendOpenGameToServer(bool openGame) {
	if (!isReady())
		return false;
	sendDataToServer(openGame, eHeader::kOpenGame);
	return true;
}

void SnakeClient::killSnake(uint16_t id) {
	if (id_ == id || (univers_.isIASnake(id) && univers_.isServer())) {
		std::lock_guard<std::mutex> guard(mutex_);
		std::cout << __PRETTY_FUNCTION__ << id << std::endl;
		(*snakeArray)[id].isAlive = false;
		sendDataToServer((*snakeArray)[id], eHeader::kSnakeUX);
	}
}

void SnakeClient::disconnect() {
	clientTCP_->disconnect();
	snakeArray = std::make_shared<SnakeArrayContainer>();
	refreshSnakeArray();
}

/***** Callback *****/


void SnakeClient::callbackGameSpeed(GameManager::eSpeed speed) {
	univers_.setBaseSpeed(speed);
}

void SnakeClient::callbackSnakeUN(const Snake &snakeUN) {
	std::lock_guard<std::mutex> guard(mutex_);
	if (snakeUN.id >= 8) return;
	(*snakeArray)[snakeUN.id] = static_cast<SnakeUN>(snakeUN);
}

void SnakeClient::callbackForcePause(int16_t) {
}


void SnakeClient::callbackSnakeUI(const Snake &snakeUI) {
	std::lock_guard<std::mutex> guard(mutex_);
	if (snakeUI.id >= 8) return;
	(*snakeArray)[snakeUI.id] = static_cast<SnakeUI>(snakeUI);
}

void SnakeClient::callbackId(uint16_t id) {
	std::lock_guard<std::mutex> guard(mutex_);

	if (id >= 8) return;
	id_ = id;
	(*snakeArray)[id_].randomSnake(id, *getSnakeArray_());
	if (fromIA_) {
		(*snakeArray)[id_].isReady = true;
		(*snakeArray)[id_].isIA = true;
	}
	sendDataToServer((*snakeArray)[id_], eHeader::kSnake);
	sendDataToServer((*snakeArray)[id_], eHeader::kSnakeUI);
}

void SnakeClient::callbackSnakeUX(const Snake &snakeUX) {
	std::lock_guard<std::mutex> guard(mutex_);
	if (snakeUX.id >= 8) return;
	(*snakeArray)[snakeUX.id] = static_cast<SnakeUX>(snakeUX);
}

void SnakeClient::callbackSnake(const Snake &snake) {
	std::lock_guard<std::mutex> guard(mutex_);
	if (snake.id >= 8) return;
	(*snakeArray)[snake.id] = snake;

	if (acceptDataFromServer()) {
		univers_.getSoundManager().playNoise(eNoise::kReadySound);
	}
}

void SnakeClient::callbackSnakeArray(const SnakeArrayContainer &snakeArrayContainer) {
	std::lock_guard<std::mutex> guard(mutex_);
	for (int index = 0; index < SNAKE_MAX; ++index) {
		if (snakeArrayContainer[index].id < 8)
			(*snakeArray)[index].deepCopy(snakeArrayContainer[index]);
	}
}

void SnakeClient::callbackBorderless(bool borderless) {
	std::lock_guard<std::mutex> guard(mutex_);
	if (acceptDataFromServer() && !univers_.isOpenGame_()) {
		univers_.setBorderless(borderless);
	}
}

void SnakeClient::callbackResizeMap(unsigned int size) {
	if (size > MAP_MAX || size < MAP_MIN) return;
	std::lock_guard<std::mutex> guard(mutex_);
	if (acceptDataFromServer() && !univers_.isOpenGame_()) {
		univers_.setMapSize(size);
		univers_.getSoundManager().playNoise(eNoise::kResizeSound);
	}
}

void SnakeClient::callbackChatInfo(ChatInfo chatInfo) {
	std::lock_guard<std::mutex> guard(mutex_);
	if (acceptDataFromServer()) {
		std::unique_ptr<Gui> &gui = univers_.getGui_();
		if (gui) gui->addMessageChat(chatInfo.toString());
	}
}

void SnakeClient::callbackStartInfo(StartInfo startInfo) {

	std::lock_guard<std::mutex> guard(mutex_);
	foodCreations.clear();
	if (acceptDataFromServer()) {
		while (univers_.getGameManager().getWorld_() == nullptr) {
			if (!univers_.isOpenGame_())
				return ;
		}
		factory_.createAllSnake(snakeArray, startInfo.nu);
		if (univers_.isServer()) {
			int max_food = (startInfo.nu > 1 ? startInfo.nu - 1 : startInfo.nu);
			for (int index = 0; index < max_food; ++index) {
				sendDataToServer(FoodInfo(PositionComponent(univers_.getGrid_().getRandomSlot(eSprite::kNone)),false, -1), eHeader::kFood);
			}
		}
		univers_.getGameManager().getWorld_()->getEventsManager().emitEvent<StartEvent>(startInfo.time_duration);
	}
}

void SnakeClient::callbackDeadConnection() {
	univers_.setOpenGame_(false);
	auto &gui = univers_.getGui_();
	if (gui && !fromIA_) {
		gui->addMessageChat(eColorLog::kOrange, "Connection has been closed.");
	}
	if (clientTCP_ && clientTCP_->isConnect()) {
		clientTCP_->disconnect();
	}
}

void SnakeClient::callbackFood(FoodInfo foodInfo) {
	std::lock_guard<std::mutex> guard(mutex_);
	if (acceptDataFromServer()) {
		foodCreations.push_back( { foodInfo.positionComponent, foodInfo.fromSnake } );
	}
}


void SnakeClient::callbackPock(uint32_t deltaTime) {

	std::lock_guard<std::mutex> guard(mutex_);
	if (acceptDataFromServer()) {
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		univers_.setMicroSecDeltaTime(univers_.getMicroSecDeltaTime() - deltaTime);
		const std::shared_ptr<KINU::World> &world = univers_.getGameManager().getWorld_();
		DirectionArray directions;
		size_t n = 0;
		std::generate(directions.begin(), directions.end(), [&n, this]{ return (*snakeArray)[n++].direction;});
		const std::shared_ptr<KINU::World> & world_= univers_.getGameManager().getWorld_();
		if (world_ != nullptr)
			world_->getEventsManager().emitEvent<DirectionArray>(directions);

		if (world) world->getEventsManager().emitEvent<NextFrame>();


	}
}

void SnakeClient::callbackOpenGame(bool open) {
	std::lock_guard<std::mutex> guard(mutex_);
	if (acceptDataFromServer() ) {
		univers_.setOpenGame_(open);
	}
}

void SnakeClient::callbackPause(eAction) {
	std::lock_guard<std::mutex> guard(mutex_);
	univers_.getGameManager().refreshTimerLoopWorld();
}

void SnakeClient::callbackCloseConnection(char) {
	if (isOpen())
		disconnect();
}

void SnakeClient::build() {

	boost::weak_ptr<SnakeClient> thisWeakPtr(shared_from_this());

	clientTCP_ = KNW::ClientTCP::create(univers_.getIoManager(),
			[thisWeakPtr]() { auto myPtr = thisWeakPtr.lock();
			if (myPtr) myPtr->callbackDeadConnection(); });


	/* Callback Snake */
	clientTCP_->getDataTCP_().addDataType<Snake >(
			([thisWeakPtr](const Snake snakeUI)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnakeUI(snakeUI); }),
			eHeader::kSnakeUI);

	clientTCP_->getDataTCP_().addDataType<Snake >(
			([thisWeakPtr](const Snake snakeUX)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnakeUX(snakeUX); }),
			eHeader::kSnakeUX);

	clientTCP_->getDataTCP_().addDataType<Snake >(
			([thisWeakPtr](const Snake snake)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnake(snake); }),
			eHeader::kSnake);

	clientTCP_->getDataTCP_().addDataType<Snake >(
			([thisWeakPtr](const Snake snakeUN)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnakeUN(snakeUN); }),
			eHeader::kSnakeUN);

	/* Callback (*snakeArray) */


	clientTCP_->getDataTCP_().addDataType<SnakeArrayContainer>(
			([thisWeakPtr](const SnakeArrayContainer snakeArrayContainer)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnakeArray(snakeArrayContainer); }),
			eHeader::kSnakeArray);

	clientTCP_->getDataTCP_().addDataType<bool>(
			([thisWeakPtr](const bool &borderless)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackBorderless(borderless); }),
			eHeader::kBorderless);

	clientTCP_->getDataTCP_().addDataType<unsigned int>(
			([thisWeakPtr](const unsigned int &mapSize)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackResizeMap(mapSize); }),
			eHeader::kResizeMap);

	clientTCP_->getDataTCP_().addDataType<bool>(
			([thisWeakPtr](const bool &openGame)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackOpenGame(openGame); }),
			eHeader::kOpenGame);

	clientTCP_->getDataTCP_().addDataType<ChatInfo>(
			([thisWeakPtr](ChatInfo chatInfo)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackChatInfo(chatInfo); }),
			eHeader::kChat);

	clientTCP_->getDataTCP_().addDataType<FoodInfo>(
			([thisWeakPtr](FoodInfo foodInfo)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackFood(foodInfo); }),
			eHeader::kFood);

	clientTCP_->getDataTCP_().addDataType<int16_t>(
			nullptr,eHeader::kForcePause);

	clientTCP_->getDataTCP_().addDataType<eAction >(
			([thisWeakPtr](eAction e) { auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackPause(e); }),
			eHeader::kPause);

	clientTCP_->getDataTCP_().addDataType<char>(
			([thisWeakPtr](char c) { auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackCloseConnection(c); }),
			eHeader::kCloseConnection);

	clientTCP_->getDataTCP_().addDataType<uint32_t>(
			([thisWeakPtr](uint32_t delta) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackPock(delta);
			}),
			eHeader::kPock);

	clientTCP_->getDataTCP_().addDataType<StartInfo>(
			([thisWeakPtr](StartInfo startInfo) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackStartInfo(startInfo);
			}),
			eHeader::kStartGame);

	clientTCP_->getDataTCP_().addDataType<uint16_t >(
			([thisWeakPtr](uint16_t id) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackId(id);
			}),
			eHeader::kId);

	clientTCP_->getDataTCP_().addDataType<char>(
			nullptr, eHeader::kShowScore);

	clientTCP_->getDataTCP_().addDataType<GameManager::eSpeed >(
			([thisWeakPtr](GameManager::eSpeed speed) { auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackGameSpeed(speed); }),
			eHeader::kGameSpeed);

}
