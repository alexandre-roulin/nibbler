#pragma once

#include <KNetwork/ClientTCP.hpp>
#include "SnakeClient.hpp"
#include "Data.hpp"
#include <network/Data.hpp>
#include <events/FoodCreation.hpp>
#include <factory/Factory.hpp>
#include <cores/Snake.hpp>
#include <nibbler.hpp>
#include "ISnakeNetwork.hpp"
#include <cores/GameManager.hpp>
class Univers;

class SnakeClient
		: public boost::enable_shared_from_this<SnakeClient>,
		  public ISnakeNetwork {
public:

	using boost_shared_ptr = boost::shared_ptr<SnakeClient>;
	using boost_weak_ptr = boost::weak_ptr<SnakeClient>;

	SnakeClient() = delete;
	virtual ~SnakeClient();
	SnakeClient &operator=(const SnakeClient &) = delete;
	SnakeClient(const SnakeClient &) = delete;

	static boost_shared_ptr create(Univers &univers, bool fromIA);

	void sendDirection(eDirection direction);

	template<typename T>
	void sendDataToServer(T data, eHeader header);

	virtual void notifyBorderless();

	virtual void notifyMapSize();

	virtual bool isOpen() const;

	virtual std::shared_ptr<SnakeArrayContainer> getSnakeArray_() const;

	virtual bool allSnakeIsDead() const ;

	virtual bool allSnakeIsReady() const;

	void lock();

	void unlock();

	void deliverEvents();

	bool isIa() const;

	uint16_t getId_() const;

	bool isSwitchingLibrary() const;

	const Snake &getSnake() const;

	void changeName(std::string const &name);

	void changeSprite(eSprite);

	void killSnake(uint16_t id);

	void changeStateReady(bool change);

	bool isReady() const;

	void connect(std::string dns, std::string port);

	void disconnect();

	bool sendOpenGameToServer(bool openGame);

	virtual void notifyGameSpeed();

	void quitGame();
private:
	SnakeClient(Univers &univers, bool fromIA);

	void build();

	void callbackGameSpeed(GameManager::eSpeed speed);

	void callbackBorderless(bool);

	void callbackSnakeUI(const Snake &);

	void callbackSnakeUX(const Snake &);

	void callbackSnake(const Snake &);

	void callbackSnakeUN(const Snake &);

	void callbackOpenGame(bool openGame);

	void callbackResizeMap(unsigned int mapSize);

	void callbackChatInfo(ChatInfo);

	void callbackFood(FoodInfo);

	void callbackForcePause(int16_t);

	void callbackPause(eAction);

	void callbackSnakeArray(const SnakeArrayContainer &);

	void callbackStartInfo(StartInfo);

	void callbackDeadConnection();

	void callbackPock(uint32_t);

	void callbackCloseConnection(char);

	void callbackId(uint16_t);

	bool acceptDataFromServer() const;

	void refreshSnakeArray();

	boost::shared_ptr<KNW::ClientTCP> clientTCP_;
	Univers &univers_;
	bool fromIA_;
	uint16_t id_;
	std::mutex mutex_;
	Factory factory_;
	std::string name_;
	eSprite sprite_;
	bool nameSet_;
	bool spriteSet_;
	std::vector<FoodCreation> foodCreations;
	std::shared_ptr<SnakeArrayContainer> snakeArray;
};


template<typename T>
void SnakeClient::sendDataToServer(T data, eHeader header) {
	clientTCP_->writeDataToServer(std::move(data), static_cast<uint16_t>(header));
}
