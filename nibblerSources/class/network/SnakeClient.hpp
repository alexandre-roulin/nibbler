#pragma once

#include <KNW/ClientTCP.hpp>
#include "SnakeClient.hpp"
#include "Data.hpp"
#include <network/Data.hpp>
#include <events/FoodCreation.hpp>
#include <factory/Factory.hpp>
#include <cores/Snake.hpp>
#include <nibbler.hpp>

class Univers;

class SnakeClient : public boost::enable_shared_from_this<SnakeClient> {
public:

	using boost_shared_ptr = boost::shared_ptr<SnakeClient>;
	using boost_weak_ptr = boost::weak_ptr<SnakeClient>;

	static boost_shared_ptr create(Univers &univers, bool fromIA);

	template<typename T>
	void sendDataToServer(T &&data, eHeader header);

	void lock();

	void unlock();

	bool allSnakeIsDead() const;

	bool allSnakeIsReady() const;

	void deliverEvents();

	bool isConnect() const;

	bool isIa() const;

	uint16_t getId_() const;

	bool isSwitchingLibrary() const;

	const SnakeArrayContainer &getSnakeArray_() const;

	const Snake &getSnake() const;

	void changeName(std::string const &name);

	void changeSprite(eSprite);

	void changeMapSize(unsigned int);

	void killSnake(uint16_t id);

	void changeStateReady(bool change);

	void changeIsBorderless(bool borderless);

	bool isReady() const;

	void connect(std::string dns, std::string port);

	void disconnect();

	void addScore(uint16_t id, eScore score);

	virtual ~SnakeClient();

private:
	SnakeClient(Univers &univers, bool fromIA);

	void build();


	void callbackRemoveSnake(int16_t);

	void callbackDeadConnection();

	void callbackInput(InputInfo);

	void callbackPock(char);

	void callbackForcePause(int16_t);

	void callbackPause(eAction);

	void callbackOpenGame(bool);

	void callbackSnake(Snake &);

	void callbackResizeMap(unsigned int);

	void callbackSnakeArray(SnakeArrayContainer &);

	void callbackBorderless(bool);

	void callbackFood(FoodInfo);

	void callbackStartInfo(StartInfo);

	void callbackChatInfo(ChatInfo);

	void callbackId(int16_t);

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
	SnakeArrayContainer snake_array_;
	std::vector<FoodCreation> foodCreations;
};


template<typename T>
void SnakeClient::sendDataToServer(T &&data, eHeader header) {
	clientTCP_->writeDataToServer(std::move(data), static_cast<uint16_t>(header));
}
