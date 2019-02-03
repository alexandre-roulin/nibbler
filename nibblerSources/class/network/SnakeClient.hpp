#pragma once

#include <KNW/ClientTCP.hpp>
#include "SnakeClient.hpp"
#include "Data.hpp"
#include <network/Data.hpp>
#include <events/FoodCreation.hpp>
#include <factory/Factory.hpp>

class Univers;

class SnakeClient {
public:
	SnakeClient(Univers &univers, bool fromIA);

	template<typename T>
	void sendDataToServer(T data, eHeaderK header);

	void lock();

	void unlock();

	bool allSnakeIsDead() const;

	bool allSnakeIsReady() const;

	void deliverEvents();

	bool isConnect() const;

	uint16_t getId_() const;

	bool isSwitchingLibrary() const;

	const std::array<Snake, SNAKE_MAX> &getSnakeArray_() const;

	const Snake &getSnake() const;

	void changeName(std::string const &name);

	void changeSprite(eSprite);

	void changeMapSize(unsigned int);

	void sendHostOpenGame();

	void killSnake(uint16_t id);
	void changeStateReady();

	void connect(std::string dns, std::string port);

private:
	void callbackInput(InputInfo);

	void callbackPock(char);

	void callbackForcePause(int16_t);

	void callbackPause(eAction);

	void callbackOpenGame(bool);

	void callbackSnake(Snake);

	void callbackResizeMap(unsigned int);

public:
	virtual ~SnakeClient();

private:

	void callbackSnakeArray(std::array<Snake, SNAKE_MAX>);

	void callbackBorderless(bool);

	void callbackFood(FoodInfo);

	void callbackStartInfo(StartInfo);

	void callbackChatInfo(ChatInfo);

	void callbackId(int16_t);

	bool acceptDataFromServer() const;

	KNW::ClientTCP clientTCP_;
	Univers &univers_;
	bool fromIA_;
	bool openGame_;
	uint16_t id_;
	std::mutex mutex_;
	Factory factory_;
	std::array<Snake, SNAKE_MAX> snake_array_;
	std::vector<FoodCreation> foodCreations;
};


template<typename T>
void SnakeClient::sendDataToServer(T data, eHeaderK header) {
	clientTCP_.writeDataToServer(std::move(data),
								 static_cast<uint16_t>(header));
}

