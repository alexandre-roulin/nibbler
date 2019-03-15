#pragma once

#include "GameManager.hpp"
#include <vector>
#include <KINU/World.hpp>
#include <events/NextFrame.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/thread.hpp>
class Univers;

class GameManager {
public:

	enum eSpeed : uint32_t {
		Easy = 120000,
		Medium = 90000,
		Hard = 60000,
		Impossible = 3000,
	};
	GameManager(Univers &univers);
	GameManager() = delete;
	~GameManager() = default;
	GameManager &operator=(const GameManager &) = delete;
	GameManager(const GameManager &) = delete;

	void startNewGame();

	void loopGame();

	void loopWorld();

	void manageGlobalInput();

	void loopUI();

	void refreshTimerLoopWorld();

	void finishGame();

	const std::shared_ptr<KINU::World> &getWorld_();

	static const uint32_t ScaleByFrame;
	static const uint32_t ScaleByRealFood;
	static const uint32_t ScaleByFakeFood;

private:
	boost::thread threadWorldLoop_;
	Univers &univers_;
	boost::asio::deadline_timer timer_loop;
	std::shared_ptr<KINU::World> world_;
};


