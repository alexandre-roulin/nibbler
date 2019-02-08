#include "SnakeClient.hpp"
#include <gui/Core.hpp>
#include <events/FoodCreation.hpp>
#include <KINU/World.hpp>
#include <events/StartEvent.hpp>
#include <Univers.hpp>

SnakeClient::SnakeClient(
		Univers &univers,
		bool fromIA_
		) :
		clientTCP_(nullptr),
		univers_(univers),
		fromIA_(fromIA_),
		id_(0),
		factory_(univers) {
}

SnakeClient::~SnakeClient() = default;

/***** Network Management *****/


void SnakeClient::connect(std::string dns, std::string port) {
	clientTCP_->connect(dns, port);
}


bool SnakeClient::acceptDataFromServer() const {
	return !fromIA_ || (univers_.isOnlyIA() && univers_.getSnakeClient()->getId_() == id_);
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
			[](Snake const &snake){
				return snake.isValid && snake.isAlive;
	});
}

void SnakeClient::deliverEvents() {
	mutex_.lock();
	for (auto foodCreation : foodCreations) {
		univers_.getWorld_().getEventsManager().emitEvent(foodCreation);
	}
	foodCreations.clear();
	mutex_.unlock();
}

bool SnakeClient::isSwitchingLibrary() const {
	return snake_array_[id_].isSwitchingLibrary;
}

const std::array<Snake, SNAKE_MAX> &SnakeClient::getSnakeArray_() const {
	return snake_array_;
}

const Snake &SnakeClient::getSnake() const {
	log_fatal("%s %d", __PRETTY_FUNCTION__, id_);
	assert(id_ >= 0 && id_ < SNAKE_MAX);
	return snake_array_[id_];
}

void SnakeClient::changeName(std::string const &name) {
	bzero(snake_array_[id_].name, NAME_BUFFER);
	if (strlen(name.c_str()) > NAME_BUFFER)
		strncpy(snake_array_[id_].name, name.c_str(), NAME_BUFFER - 1);
	else
		strcpy(snake_array_[id_].name, name.c_str());
	sendDataToServer(snake_array_[id_], eHeader::kSnake);
}

void SnakeClient::changeMapSize(unsigned int size) {
	sendDataToServer(size, eHeader::kResizeMap);
}

bool SnakeClient::allSnakeIsReady() const {
	return std::none_of(snake_array_.begin(), snake_array_.end(),
								  [](Snake const &snake){
									  return snake.isValid && snake.isReady;
								  });;
}

void SnakeClient::sendHostOpenGame() {
//	sendDataToServer(true, eHeaderK::kOpenGame);
}

void SnakeClient::changeSprite(eSprite snakeSprite) {
	snake_array_[id_].sprite = snakeSprite;

	sendDataToServer(snake_array_[id_], eHeader::kSnake);
}


void SnakeClient::changeStateReady(bool change) {
	snake_array_[id_].isReady = change;
	sendDataToServer(snake_array_[id_], eHeader::kSnake);
}

void SnakeClient::changeIsBorderless(bool borderless) {
	sendDataToServer(borderless, eHeader::kBorderless);
}


bool SnakeClient::isConnect() const {
	if (clientTCP_)
		return clientTCP_->isConnect();
	else
		return false;
}

void SnakeClient::killSnake(uint16_t id) {
	log_success("%s ID : %d C{%d, %d}", __PRETTY_FUNCTION__, id, id_ == id, fromIA_);
	if (id_ == id || (univers_.isIASnake(id) && univers_.isServer())) {
		snake_array_[id].isAlive = false;
		sendDataToServer(snake_array_[id], eHeader::kSnake);
	}

}
void SnakeClient::removeSnakeFromGame() {
	sendDataToServer(id_, eHeader::kRemoveSnake);
}


void SnakeClient::disconnect() {
	clientTCP_->disconnect();
	clientTCP_.reset();
	build();
}

/***** Callback *****/

void SnakeClient::callbackRemoveSnake(int16_t) {

}

void SnakeClient::callbackDeadConnection() {
	log_success("%s", __PRETTY_FUNCTION__ );
	snake_array_[id_].isReady = false;
	univers_.setOpenGame_(false);
	if (clientTCP_ != nullptr) {
//		std::cout << "disconnect" <<std::endl;
//		clientTCP_->disconnect();
		log_info("Rest client %d", clientTCP_.use_count());
		clientTCP_.reset();
		build();
	}
}

void SnakeClient::callbackPock(char) {
	log_success("%s %d", __PRETTY_FUNCTION__, acceptDataFromServer());
	mutex_.lock();
	if (acceptDataFromServer() && univers_.isOpenGame_()) {
		univers_.getWorld_().getEventsManager().emitEvent<NextFrame>();
	}
	mutex_.unlock();
}

void SnakeClient::callbackResizeMap(unsigned int size) {
	log_success("%s", __PRETTY_FUNCTION__ );
	mutex_.lock();
	if (acceptDataFromServer()) {
		univers_.setMapSize(size);
		univers_.playNoise(eNoise::kResizeSound);
	}
	mutex_.unlock();
}

void SnakeClient::callbackOpenGame(bool open) {
	log_success("%s", __PRETTY_FUNCTION__ );
	mutex_.lock();
	if (acceptDataFromServer()) {
		univers_.setOpenGame_(open);
	}
	mutex_.unlock();
}

void SnakeClient::callbackBorderless(bool borderless) {
	log_success("%s", __PRETTY_FUNCTION__ );
	mutex_.lock();
	if (acceptDataFromServer()) {
		univers_.setBorderless(borderless);
	}
	mutex_.unlock();
}

void SnakeClient::callbackId(int16_t id) {
	log_success("%s id : %d id_ : %d", __PRETTY_FUNCTION__ , id, id_);
	mutex_.lock();
	id_ = id;

	if (fromIA_) {
		changeStateReady(true);
	}
	mutex_.unlock();
}

void SnakeClient::callbackInput(InputInfo) {
	log_success("%s", __PRETTY_FUNCTION__ );

}

void SnakeClient::callbackForcePause(int16_t) {
	log_success("%s", __PRETTY_FUNCTION__ );

}

void SnakeClient::callbackPause(eAction) {
	log_success("%s", __PRETTY_FUNCTION__ );
	mutex_.lock();
	univers_.refreshTimerLoopWorld();
	mutex_.unlock();
}

void SnakeClient::callbackSnake(Snake snake) {
	log_success("%s", __PRETTY_FUNCTION__ );
	mutex_.lock();
	snake_array_[snake.id] = snake;
	if (acceptDataFromServer()) {
		univers_.playNoise(eNoise::kReadySound);
	}
	mutex_.unlock();
}

void SnakeClient::callbackFood(FoodInfo foodInfo) {
	log_success("%s", __PRETTY_FUNCTION__ );
	mutex_.lock();
	if (acceptDataFromServer()) {
		foodCreations.push_back(
				FoodCreation(foodInfo.positionComponent,foodInfo.fromSnake));
	}
	mutex_.unlock();
}

void SnakeClient::callbackStartInfo(StartInfo startInfo) {
	log_success("%s", __PRETTY_FUNCTION__ );
	mutex_.lock();
	foodCreations.clear();
	if (acceptDataFromServer()) {
		factory_.createAllSnake(snake_array_, startInfo.nu);
		if (univers_.isServer()) {
			int max_food = (startInfo.nu > 1 ? startInfo.nu - 1 : startInfo.nu);
			for (int index = 0; index < max_food; ++index) {
				clientTCP_->writeDataToServer(
						FoodInfo(
								PositionComponent(
										univers_.getGrid_().getRandomSlot(eSprite::kNone)),false),
										static_cast<uint16_t>(eHeader::kFood)
										);
			}
		}
		univers_.getWorld_().getEventsManager().emitEvent<StartEvent>(startInfo.time_duration);
	}
	mutex_.unlock();
}

void SnakeClient::callbackChatInfo(ChatInfo chatInfo) {
	log_success("%s", __PRETTY_FUNCTION__ );
	mutex_.lock();
	if (acceptDataFromServer()) {
		univers_.getCore_().addMessageChat(chatInfo.toString());
	}
	mutex_.unlock();
}

void SnakeClient::callbackSnakeArray(std::array<Snake, SNAKE_MAX> new_snake_array) {
	log_success("%s %d", __PRETTY_FUNCTION__, snake_array_[id_].isReady);
	mutex_.lock();
	snake_array_ = new_snake_array;
	mutex_.unlock();
}

bool SnakeClient::isReady() const {
	return snake_array_[id_].isReady;
}

SnakeClient::boost_shared_ptr SnakeClient::create(Univers &univers, bool fromIA) {
	auto ptr = boost::shared_ptr<SnakeClient>(new SnakeClient(univers, fromIA));
	ptr->build();
	return ptr;
}

void SnakeClient::build() {

	boost::weak_ptr<SnakeClient> thisWeakPtr(shared_from_this());

	clientTCP_ = KNW::ClientTCP::create(
			[thisWeakPtr]()
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackDeadConnection(); });

	clientTCP_->addDataType<int16_t >(
			([thisWeakPtr](int16_t id)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackRemoveSnake(id); }),
			eHeader::kRemoveSnake);

	clientTCP_->addDataType<InputInfo>(
			([thisWeakPtr](InputInfo ii)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackInput(ii); }),
			eHeader::kInput);

	clientTCP_->addDataType<std::array<Snake, SNAKE_MAX>>(
			([thisWeakPtr](std::array<Snake, SNAKE_MAX> snake_array)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnakeArray(snake_array); }),
			eHeader::kSnakeArray);

	clientTCP_->addDataType<char>(
			([thisWeakPtr](char c)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackPock(c); }),
			eHeader::kPock);

	clientTCP_->addDataType<bool>(
			([thisWeakPtr](bool borderless)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackBorderless(borderless); }),
			eHeader::kBorderless);

	clientTCP_->addDataType<unsigned int>(
			([thisWeakPtr](unsigned int mapSize)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackResizeMap(mapSize); }),
			eHeader::kResizeMap);

	clientTCP_->addDataType<bool>(
			([thisWeakPtr](bool openGame)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackOpenGame(openGame); }),
			eHeader::kOpenGame);

	clientTCP_->addDataType<int16_t>(
			([thisWeakPtr](int16_t id)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackId(id); }),
			eHeader::kId);

	clientTCP_->addDataType<ChatInfo>(
			([thisWeakPtr](ChatInfo chatInfo)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackChatInfo(chatInfo); }),
			eHeader::kChat);

	clientTCP_->addDataType<StartInfo>(
			([thisWeakPtr](StartInfo startInfo)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackStartInfo(startInfo); }),
			eHeader::kStartGame);

	clientTCP_->addDataType<FoodInfo>(
			([thisWeakPtr](FoodInfo foodInfo)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackFood(foodInfo); }),
			eHeader::kFood);

	clientTCP_->addDataType<Snake>(
			([thisWeakPtr](Snake snake)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackSnake(snake); }),
			eHeader::kSnake);

	clientTCP_->addDataType<int16_t>(
			([thisWeakPtr](int16_t id)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackForcePause(id); }),
			eHeader::kForcePause);

	clientTCP_->addDataType<eAction >(
			([thisWeakPtr](eAction e)
			{ auto myPtr = thisWeakPtr.lock(); if(myPtr) myPtr->callbackPause(e); }),
			eHeader::kPause);
}

