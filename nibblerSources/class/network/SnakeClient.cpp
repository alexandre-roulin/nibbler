#include "SnakeClient.hpp"
#include <gui/Gui.hpp>
#include <KINU/World.hpp>
#include <events/StartEvent.hpp>
#include <cores/GameManager.hpp>

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
		spriteSet_(false) {
}

SnakeClient::~SnakeClient() = default;

/***** Network Management *****/


void SnakeClient::connect(std::string dns, std::string port) {
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

/***** Snake Management *****/


uint16_t SnakeClient::getId_() const {
	return id_;
}

bool SnakeClient::allSnakeIsDead() const {
	return std::none_of(snake_array_.begin(), snake_array_.end(),
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
	return snake_array_[id_].isSwitchingLibrary;
}

const SnakeArrayContainer &SnakeClient::getSnakeArray_() const {
	return snake_array_;
}

const Snake &SnakeClient::getSnake() const {
	return snake_array_[id_];
}

void SnakeClient::changeName(std::string const &name) {

	nameSet_ = true;
	//name_ = std::string(NAME_BUFFER, '\0');
	//name_.assign(name, NAME_BUFFER);
	name_ = name;
	refreshSnakeArray();
	sendDataToServer(snake_array_[id_], eHeader::kSnake);
}


void SnakeClient::notifyBorderless() {
	sendDataToServer(univers_.isBorderless(), eHeader::kBorderless);
}

void SnakeClient::notifyMapSize() {
	sendDataToServer(univers_.getMapSize(), eHeader::kResizeMap);
}

bool SnakeClient::allSnakeIsReady() const {
	return std::none_of(snake_array_.begin(), snake_array_.end(),
						[](Snake const &snake) {
							return snake.isValid && snake.isReady;
						});;
}

void SnakeClient::changeSprite(eSprite snakeSprite) {
	spriteSet_ = true;
	sprite_ = snakeSprite;
	refreshSnakeArray();
	sendDataToServer(snake_array_[id_], eHeader::kSnake);
}


void SnakeClient::changeStateReady(bool change) {
	snake_array_[id_].isReady = change;
	sendDataToServer(snake_array_[id_], eHeader::kSnake);
}


bool SnakeClient::isOpen() const {
	return clientTCP_ != nullptr && clientTCP_->isConnect();
}

bool SnakeClient::isIa() const {
	return fromIA_;
}

void SnakeClient::killSnake(uint16_t id) {
	if (id_ == id || (univers_.isIASnake(id) && univers_.isServer())) {
		log_debug("%s id[%d]", __PRETTY_FUNCTION__, id);
		snake_array_[id].isAlive = false;
		sendDataToServer(snake_array_[id], eHeader::kSnake);
	}

}

void SnakeClient::addScore(uint16_t id, eScore score) {
	log_info("%s %d", __PRETTY_FUNCTION__, snake_array_[id].isUpdate);
	snake_array_[id].score_ += score;
	uint32_t timeLess = 0;
	uint32_t actualMicroSec = univers_.getMicroSecDeltaTime();
	switch (score) {
		case kFromTime :
			timeLess =  actualMicroSec - actualMicroSec / 10000;
			break;
		case kFromSnakeFood :
			timeLess = actualMicroSec - GameManager::ScaleByFakeFood;
			break;
		case kFromFood :
			timeLess = actualMicroSec - GameManager::ScaleByRealFood;
			break;
	}
	univers_.setMicroSecDeltaTime(timeLess);
	sendDataToServer(snake_array_[id], eHeader::kSnake);
}

void SnakeClient::disconnect() {
	clientTCP_->disconnect();
	std::for_each(snake_array_.begin(), snake_array_.end(), [](Snake &snake){ snake.reset(); });
	refreshSnakeArray();
}

/***** Callback *****/

void SnakeClient::callbackRemoveSnake(int16_t) {}

void SnakeClient::callbackDeadConnection() {
	log_debug("%s", __PRETTY_FUNCTION__);
	snake_array_[id_].isReady = false;
	refreshSnakeArray();
	univers_.setOpenGame_(false);
	if (clientTCP_ != nullptr) {
		clientTCP_->disconnect();
	}
}

void SnakeClient::callbackPock(char) {
	log_info("%s %d", __PRETTY_FUNCTION__, snake_array_[id_].isUpdate);
	std::lock_guard<std::mutex> guard(mutex_);
	if (acceptDataFromServer() && univers_.isOpenGame_()) {
		const std::shared_ptr<KINU::World> &world = univers_.getGameManager().getWorld_();
		if (world)
			world->getEventsManager().emitEvent<NextFrame>();
	}
}

void SnakeClient::callbackResizeMap(unsigned int size) {
	std::lock_guard<std::mutex> guard(mutex_);
	if (acceptDataFromServer()) {
		log_info("%s %d", __PRETTY_FUNCTION__, snake_array_[id_].isUpdate);
		univers_.setMapSize(size);
		univers_.getSoundManager().playNoise(eNoise::kResizeSound);
	}
}

void SnakeClient::callbackOpenGame(bool open) {
	std::lock_guard<std::mutex> guard(mutex_);
	if (acceptDataFromServer()) {
		log_info("%s %d", __PRETTY_FUNCTION__, snake_array_[id_].isUpdate);
		univers_.setOpenGame_(open);
	}
}

void SnakeClient::callbackBorderless(bool borderless) {
	std::lock_guard<std::mutex> guard(mutex_);
	if (acceptDataFromServer()) {
		log_info("%s %d", __PRETTY_FUNCTION__, snake_array_[id_].isUpdate);
		univers_.setBorderless(borderless);
	}
}

void SnakeClient::callbackId(int16_t id) {
	log_debug("%s id : %d id_ : %d", __PRETTY_FUNCTION__, id, id_);
	std::lock_guard<std::mutex> guard(mutex_);
	id_ = id;
	if (fromIA_) {
		snake_array_[id_].id_ = id;
		snake_array_[id_].isReady = true;
		snake_array_[id_].isIA = true;
	}
	snake_array_[id_].isReadyToExpose = true;
	refreshSnakeArray();
	sendDataToServer(snake_array_[id_], eHeader::kSnake);
}

void SnakeClient::callbackInput(InputInfo) {
	log_debug("%s", __PRETTY_FUNCTION__);
}

void SnakeClient::callbackForcePause(int16_t) {
	log_debug("%s", __PRETTY_FUNCTION__);
}

void SnakeClient::callbackPause(eAction) {
	log_debug("%s", __PRETTY_FUNCTION__);
	std::lock_guard<std::mutex> guard(mutex_);
	univers_.getGameManager().refreshTimerLoopWorld();
}

void SnakeClient::callbackSnake(Snake &snake) {
	std::lock_guard<std::mutex> guard(mutex_);
//	log_debug("%s id[%d] isUpdate[%d]", __PRETTY_FUNCTION__, id_,
//			  snake_array_[id_].isUpdate);
	snake_array_[snake.id_] = snake;
	if (acceptDataFromServer()) {
		univers_.getSoundManager().playNoise(eNoise::kReadySound);
	}
}

void SnakeClient::callbackSnakeArray(SnakeArrayContainer &new_snake_array) {
	std::lock_guard<std::mutex> guard(mutex_);
	log_debug("%s id[%d] isUpdate[%d]", __PRETTY_FUNCTION__, id_,
			  snake_array_[id_].isUpdate);
	snake_array_ = new_snake_array;
	refreshSnakeArray();
}

void SnakeClient::callbackFood(FoodInfo foodInfo) {
	std::lock_guard<std::mutex> guard(mutex_);
	log_debug("%s id[%d] isUpdate[%d]", __PRETTY_FUNCTION__, id_,
			  snake_array_[id_].isUpdate);
	if (acceptDataFromServer()) {
		foodCreations.push_back(
				FoodCreation(foodInfo.positionComponent, foodInfo.fromSnake));
	}
}

void SnakeClient::callbackStartInfo(StartInfo startInfo) {
	std::lock_guard<std::mutex> guard(mutex_);
	foodCreations.clear();
	log_debug("%s id[%d] isUpdate[%d]", __PRETTY_FUNCTION__, id_,
			  snake_array_[id_].isUpdate);
	if (acceptDataFromServer()) {
		factory_.createAllSnake(snake_array_, startInfo.nu);
		if (univers_.isServer()) {
			int max_food = (startInfo.nu > 1 ? startInfo.nu - 1 : startInfo.nu);
			for (int index = 0; index < max_food; ++index) {
				log_success("IsConnect() ? %d", clientTCP_->isConnect());
				clientTCP_->writeDataToServer(
						FoodInfo(
								PositionComponent(
										univers_.getGrid_().getRandomSlot(
												eSprite::kNone)), false),
						static_cast<uint16_t>(eHeader::kFood)
				);
			}
		}
		const std::shared_ptr<KINU::World> &world = univers_.getGameManager().getWorld_();
		if (world)
			world->getEventsManager().emitEvent<StartEvent>(startInfo.time_duration);
	}
}

void SnakeClient::callbackChatInfo(ChatInfo chatInfo) {
	std::lock_guard<std::mutex> guard(mutex_);
	if (acceptDataFromServer()) {
		log_debug("%s", __PRETTY_FUNCTION__);
		univers_.getGui_()->addMessageChat(chatInfo.toString());
	}
}

bool SnakeClient::isReady() const {
	return snake_array_[id_].isReady;
}

SnakeClient::boost_shared_ptr
SnakeClient::create(Univers &univers, bool fromIA) {
	auto ptr = boost::shared_ptr<SnakeClient>(new SnakeClient(univers, fromIA));
	ptr->build();
	return ptr;
}

void SnakeClient::refreshSnakeArray() {
	if (nameSet_)
		std::memcpy(snake_array_[id_].name, name_.c_str(), NAME_BUFFER);
	if (spriteSet_)
		snake_array_[id_].sprite = sprite_;
}


void SnakeClient::callbackCloseConnection(char) {
	disconnect();
}

void SnakeClient::build() {

	boost::weak_ptr<SnakeClient> thisWeakPtr(shared_from_this());

	clientTCP_ = KNW::ClientTCP::create(univers_.getIoManager(),
										[thisWeakPtr]() {
											auto myPtr = thisWeakPtr.lock();
											if (myPtr) myPtr->callbackDeadConnection();
										});

	clientTCP_->getDataTCP_().addDataType<int16_t>(
			([thisWeakPtr](int16_t id) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackRemoveSnake(id);
			}),
			eHeader::kRemoveSnake);

	clientTCP_->getDataTCP_().addDataType<InputInfo>(
			([thisWeakPtr](InputInfo ii) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackInput(ii);
			}),
			eHeader::kInput);

	clientTCP_->getDataTCP_().addDataType<std::array<Snake, SNAKE_MAX>>(
			([thisWeakPtr](SnakeArrayContainer &snake_array) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackSnakeArray(snake_array);
			}),
			eHeader::kSnakeArray);

	clientTCP_->getDataTCP_().addDataType<char>(
			([thisWeakPtr](char c) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackPock(c);
			}),
			eHeader::kPock);

	clientTCP_->getDataTCP_().addDataType<bool>(
			([thisWeakPtr](bool borderless) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackBorderless(borderless);
			}),
			eHeader::kBorderless);

	clientTCP_->getDataTCP_().addDataType<unsigned int>(
			([thisWeakPtr](unsigned int mapSize) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackResizeMap(mapSize);
			}),
			eHeader::kResizeMap);

	clientTCP_->getDataTCP_().addDataType<bool>(
			([thisWeakPtr](bool openGame) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackOpenGame(openGame);
			}),
			eHeader::kOpenGame);

	clientTCP_->getDataTCP_().addDataType<int16_t>(
			([thisWeakPtr](int16_t id) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackId(id);
			}),
			eHeader::kId);

	clientTCP_->getDataTCP_().addDataType<ChatInfo>(
			([thisWeakPtr](ChatInfo chatInfo) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackChatInfo(chatInfo);
			}),
			eHeader::kChat);

	clientTCP_->getDataTCP_().addDataType<StartInfo>(
			([thisWeakPtr](StartInfo startInfo) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackStartInfo(startInfo);
			}),
			eHeader::kStartGame);

	clientTCP_->getDataTCP_().addDataType<FoodInfo>(
			([thisWeakPtr](FoodInfo foodInfo) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackFood(foodInfo);
			}),
			eHeader::kFood);

	clientTCP_->getDataTCP_().addDataType<Snake>(
			([thisWeakPtr](Snake &snake) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackSnake(snake);
			}),
			eHeader::kSnake);

	clientTCP_->getDataTCP_().addDataType<int16_t>(
			([thisWeakPtr](int16_t id) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackForcePause(id);
			}),
			eHeader::kForcePause);

	clientTCP_->getDataTCP_().addDataType<eAction>(
			([thisWeakPtr](eAction e) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackPause(e);
			}),
			eHeader::kPause);

	clientTCP_->getDataTCP_().addDataType<char>(
			([thisWeakPtr](char c) {
				auto myPtr = thisWeakPtr.lock();
				if (myPtr) myPtr->callbackCloseConnection(c);
			}),
			eHeader::kCloseConnection);
}
