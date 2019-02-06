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
		clientTCP_(std::bind(&SnakeClient::callbackDeadConnection, this)),
		univers_(univers),
		fromIA_(fromIA_),
		id_(0),
		factory_(univers) {

	clientTCP_.addDataType<int16_t >(
			std::bind(&SnakeClient::callbackRemoveSnake,
					  this,
					  std::placeholders::_1)
			, eHeaderK::kRemoveSnake);

	clientTCP_.addDataType<InputInfo>(
			std::bind(&SnakeClient::callbackInput,
					  this,
					  std::placeholders::_1)
			, eHeaderK::kInput);

	clientTCP_.addDataType<InputInfo>(
			std::bind(&SnakeClient::callbackInput,
					  this,
					  std::placeholders::_1)
			, eHeaderK::kInput);

	clientTCP_.addDataType<std::array<Snake, SNAKE_MAX>>(
			std::bind(&SnakeClient::callbackSnakeArray,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kSnakeArray);

	clientTCP_.addDataType<char>(
			std::bind(&SnakeClient::callbackPock,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kPock);

	clientTCP_.addDataType<bool>(
			std::bind(&SnakeClient::callbackBorderless,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kBorderless);

	clientTCP_.addDataType<unsigned int>(
			std::bind(&SnakeClient::callbackResizeMap,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kResizeMap);

	clientTCP_.addDataType<bool>(
			std::bind(&SnakeClient::callbackOpenGame,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kOpenGame);

	clientTCP_.addDataType<int16_t>(
			std::bind(&SnakeClient::callbackId,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kId);

	clientTCP_.addDataType<ChatInfo>(
			std::bind(&SnakeClient::callbackChatInfo,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kChat);

	clientTCP_.addDataType<StartInfo>(
			std::bind(&SnakeClient::callbackStartInfo,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kStartGame);

	clientTCP_.addDataType<FoodInfo>(
			std::bind(&SnakeClient::callbackFood,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kFood);

	clientTCP_.addDataType<Snake>(
			std::bind(&SnakeClient::callbackSnake,
					  this,
					  std::placeholders::_1)
			,eHeaderK::kSnake);

	clientTCP_.addDataType<int16_t>(
			std::bind(&SnakeClient::callbackForcePause,
					  this,
					  std::placeholders::_1),
			eHeaderK::kForcePause);

	clientTCP_.addDataType<eAction >(
			std::bind(&SnakeClient::callbackPause,
					  this,
					  std::placeholders::_1),
			eHeaderK::kPause);
}

SnakeClient::~SnakeClient() = default;

/***** Network Management *****/


void SnakeClient::connect(std::string dns, std::string port) {
	clientTCP_.connect(dns, port);
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
	sendDataToServer(snake_array_[id_], eHeaderK::kSnake);
}

void SnakeClient::changeMapSize(unsigned int size) {
	sendDataToServer(size, eHeaderK::kResizeMap);
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

	sendDataToServer(snake_array_.data()[id_], eHeaderK::kSnake);
}


void SnakeClient::changeStateReady(bool change) {
	snake_array_[id_].isReady = change;
	sendDataToServer(snake_array_[id_], eHeaderK::kSnake);
}

void SnakeClient::changeIsBorderless(bool borderless) {
	sendDataToServer(borderless, eHeaderK::kBorderless);
}


bool SnakeClient::isConnect() const {
//	log_warn("%s %d", __PRETTY_FUNCTION__, clientTCP_.isConnect());
	return clientTCP_.isConnect();
}

void SnakeClient::killSnake(uint16_t id) {
	log_success("%s ID : %d C{%d, %d}", __PRETTY_FUNCTION__, id, id_ == id, fromIA_);
	if (id_ == id || fromIA_) {
		snake_array_[id].isAlive = false;
		sendDataToServer(snake_array_[id], eHeaderK::kSnake);
	}

}
void SnakeClient::removeSnakeFromGame() {
	sendDataToServer(id_, eHeaderK::kRemoveSnake);
}

/***** Callback *****/

void SnakeClient::callbackRemoveSnake(int16_t) {

}

void SnakeClient::callbackDeadConnection() {
	log_success("%s", __PRETTY_FUNCTION__ );
	univers_.setOpenGame_(false);
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
		univers_.playNoise(eSound::RESIZE_MAP);
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
	log_success("%s id : %d", __PRETTY_FUNCTION__ , id);
	mutex_.lock();
	id_ = id;
	if (fromIA_) {
		snake_array_[id] = Snake::randomSnake(id);
		changeStateReady(true);
	} else {
		snake_array_[id].id = id_;
	};
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
		univers_.playNoise(eSound::READY);
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
		factory_.create_all_snake(snake_array_, startInfo.nu);
		if (univers_.isServer()) {
			int max_food = (startInfo.nu > 1 ? startInfo.nu - 1 : startInfo.nu);
			for (int index = 0; index < max_food; ++index) {
				clientTCP_.writeDataToServer(
						FoodInfo(
								PositionComponent(
										univers_.getGrid_().getRandomSlot(eSprite::NONE)),false),
										static_cast<uint16_t>(eHeaderK::kFood)
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
	log_success("%s", __PRETTY_FUNCTION__ );
	mutex_.lock();
	snake_array_ = new_snake_array;
	mutex_.unlock();
}

bool SnakeClient::isReady() const {
	return snake_array_[id_].isReady;
}
