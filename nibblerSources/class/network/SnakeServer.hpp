#pragma once

#include "SnakeServer.hpp"
#include <KNW/ServerTCP.hpp>
#include "Data.hpp"
#include <network/Data.hpp>
#include <cores/Snake.hpp>

class Univers;

class SnakeServer : public boost::enable_shared_from_this<SnakeServer> {

public:
	SnakeServer() = delete;
	static boost::shared_ptr<SnakeServer> create(Univers &univers, unsigned int port);

	void startGame();

	bool isReady() const;

	bool isFull() const;

	unsigned short getPort_() const;

	const std::array<Snake, SNAKE_MAX> &getSnakeArray_() const;

	bool sendOpenGameToClient();

	void closeAcceptorServer();

private:
	SnakeServer(Univers &univers, unsigned int port);
	void build();
	void callbackRemoveSnake(int16_t);

	void callbackDeadConnection(size_t index);
	void callbackSnakeArray(std::array<Snake, SNAKE_MAX>);

	void callbackPock(char);

	void callbackBorderless(bool);

	void callbackResizeMap(unsigned int);

	void callbackOpenGame(bool);

	void callbackId(int16_t);

	void callbackChatInfo(ChatInfo);

	void callbackInput(InputInfo);

	void callbackForcePause(int16_t);

	void callbackPause(eAction);

	void callbackAccept(size_t);

	void callbackSnake(Snake);

	void callbackFood(FoodInfo);

	void callbackStartInfo(StartInfo);

	void updateInput();

	Univers &univers_;
	std::vector<FoodInfo> foodInfoArray;
	std::mutex mutex_;
	bool pause_;
	unsigned short port_;
	unsigned int mapSize_;
	boost::shared_ptr<KNW::ServerTCP> serverTCP_;
	std::array<Snake, SNAKE_MAX> snake_array_;
	bool borderless_;
};


