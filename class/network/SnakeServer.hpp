#pragma once

#include "SnakeServer.hpp"
#include <KNW/ServerTCP.hpp>
#include "Data.hpp"
#include <network/Data.hpp>

class Univers;

class SnakeServer {

public:
	SnakeServer(Univers &univers, unsigned int port);

	void startGame();

	bool isReady() const;

	bool isFull() const;

	unsigned short getPort_() const;

private:


	void callbackSnakeArray(std::array<Snake, MAX_SNAKE>);
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


	std::vector<FoodInfo> foodInfoArray;
	std::mutex mutex_;
	bool pause_;
	unsigned short port_;
	unsigned int mapSize_;
	std::array<int, MAX_SNAKE> connectionsId;
	KNW::ServerTCP serverTCP_;
	std::array<Snake, MAX_SNAKE> snake_array_;

};


