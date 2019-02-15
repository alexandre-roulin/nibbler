#pragma once

#include "GameManager.hpp"
#include <vector>
#include <KINU/World.hpp>
#include <events/NextFrame.hpp>
#include <boost/asio/deadline_timer.hpp>

class GameManager {
public:
	GameManager(Univers &univers);
	void startNewGame();
	void loopWorld();
	void loopUI();
	void refreshTimerLoopWorld();
	void finishGame();
	KINU::World &getWorld_();

private:
	Univers &univers_;
	std::vector<NextFrame> nextFrame;
	boost::asio::deadline_timer timer_loop;
	std::shared_ptr<KINU::World> world_;
	boost::thread_group threadGroup;
};


