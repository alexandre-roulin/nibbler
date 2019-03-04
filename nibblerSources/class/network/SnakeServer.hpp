#pragma once

#include "SnakeServer.hpp"
#include <KNW/ServerTCP.hpp>
#include "Data.hpp"
#include <network/Data.hpp>
#include <cores/Snake.hpp>
#include "ISnakeNetwork.hpp"

class Univers;

class SnakeServer : public boost::enable_shared_from_this<SnakeServer>, public ISnakeNetwork {

public:
	using b_ptr = boost::shared_ptr<SnakeServer>;
	using w_ptr = boost::weak_ptr<SnakeServer>;
	SnakeServer() = delete;
	static boost::shared_ptr<SnakeServer> create(Univers &univers, const std::string dns, unsigned short port);

	void startGame();

	bool isReady() const;

	bool isFull() const;

	unsigned short getPort_() const;

	void notifyBorderless() override;

	bool isOpen() const override;

	void notifyMapSize() override;

	std::shared_ptr<SnakeArrayContainer> getSnakeArray_() const override;

	bool allSnakeIsDead() const override;

	bool allSnakeIsReady() const override;

	bool sendOpenGameToClient(bool openGame = true);

	void closeAcceptorServer();

	virtual ~SnakeServer();

private:
	SnakeServer(Univers &univers);
	void build(const std::string dns, unsigned short port);

	void callbackAccept(size_t);

	void callbackBorderless(bool);

	void callbackSnakeUI(const Snake &);

	void callbackSnakeUX(const Snake &);

	void callbackSnake(const Snake &);

	void callbackSnakeUN(const Snake &);

	void callbackOpenGame(bool openGame);

	void callbackResizeMap(unsigned int mapSize);

	void callbackChatInfo(ChatInfo);

	void callbackFood(FoodInfo);

	void callbackForcePause(uint16_t);

	void callbackDeadConnection(size_t index);

	void callbackPause(eAction);

	void updateInput();

	Univers &univers_;
	std::vector<FoodInfo> foodInfoArray;
	std::mutex mutex_;
	bool pause_;
	boost::shared_ptr<KNW::ServerTCP> serverTCP_;
	std::shared_ptr<SnakeArrayContainer> snakeArray_;
};
