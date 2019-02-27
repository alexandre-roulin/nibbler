#include "GameManager.hpp"
#include "cores/Univers.hpp"
#include <network/SnakeServer.hpp>
#include <KINU/World.hpp>
#include <systems/MotionSystem.hpp>
#include <systems/JoystickSystem.hpp>
#include <systems/FollowSystem.hpp>
#include <systems/FoodCreationSystem.hpp>
#include <systems/CollisionSystem.hpp>
#include <systems/SpriteSystem.hpp>
#include <systems/RenderSystem.hpp>
#include <systems/FoodEatSystem.hpp>
#include <events/FoodEat.hpp>
#include <KINU/World.hpp>
#include <events/StartEvent.hpp>
#include <events/FoodCreation.hpp>
#include <events/JoystickEvent.hpp>

const uint32_t GameManager::ScaleByFrame = 10;
const uint32_t GameManager::ScaleByRealFood = 600;
const uint32_t GameManager::ScaleByFakeFood = 50;

const uint32_t GameManager::Easy = 1000000;  //0.16sec frame
const uint32_t GameManager::Medium = 120000;  //0.15sec frame
const uint32_t GameManager::Hard = 80000;  //0.1sec frame
const uint32_t GameManager::Impossible = 10000;  //0.01sec frame

GameManager::GameManager(Univers &univers)
	:
	univers_(univers),
	timer_loop(univers_.getIoManager().getIo()) {
}


void GameManager::startNewGame() {

	world_ = std::make_unique<KINU::World>();

	univers_.setGrid_(std::make_shared<MutantGrid<eSprite>>(univers_.getMapSize()));
	univers_.getGrid_().fill(eSprite::kNone);

	world_->getSystemsManager().addSystem<CollisionSystem>(univers_);
	world_->getSystemsManager().addSystem<FollowSystem>();
	world_->getSystemsManager().addSystem<JoystickSystem>(univers_);
	world_->getSystemsManager().addSystem<MotionSystem>(univers_);
	world_->getSystemsManager().addSystem<SpriteSystem>(univers_);
	world_->getSystemsManager().addSystem<RenderSystem>(univers_);
	world_->getSystemsManager().addSystem<FoodCreationSystem>();
	world_->getSystemsManager().addSystem<FoodEatSystem>();

	boost::asio::deadline_timer timer_start(univers_.getIoManager().getIo());

	std::vector<StartEvent> startEvent;
	SnakeClient::boost_shared_ptr ptr(univers_.getSnakeClient().lock());

	if (univers_.isServer()) {
		univers_.getSnakeServer().startGame();
	}

	for (;ptr && startEvent.empty();) {
		ptr->lock();
		startEvent = world_->getEventsManager().getEvents<StartEvent>();
		ptr->unlock();
	}

	world_->update();
	world_->getSystemsManager().getSystem<SpriteSystem>().update();
	world_->getSystemsManager().getSystem<RenderSystem>().update();
	univers_.updateDisplayUI();

	timer_start.expires_at(startEvent.front().start_time);
	timer_start.wait();

	if (univers_.isServer()) {
		for (auto &bobby : univers_.getBobbys()) {
			bobby->buildIA();
//			bobby->sendDirection();
		}
	}

//	univers_.manageSnakeClientInput();

	threadWorldLoop_ = boost::thread([this](){
		loopWorld();
	});
}


void GameManager::loopWorld() {
	SnakeClient::boost_shared_ptr ptr(univers_.getSnakeClient().lock());

	while (!ptr->allSnakeIsDead() && univers_.isOpenGame_() && univers_.displayIsAvailable()) {
		timer_loop.expires_from_now(boost::posix_time::microsec(univers_.getMicroSecDeltaTime()));
		timer_loop.wait();
		loopWorldWork();
	}
}


void GameManager::loopWorldWork() {
	boost::asio::thread_pool pool(8);

	SnakeClient::boost_shared_ptr ptr(univers_.getSnakeClient().lock());
	if (!ptr)
		return;
	{ //Manage Input
		Bobby::clearPriority();
		if (univers_.isServer()) {
			for (auto &bobby : univers_.getBobbys()) {
				if (world_->getEntitiesManager().hasEntityByTagId(bobby->getId() + eTag::kHeadTag)) {
					ptr->addScore(bobby->getId(), eScore::kFromTime);
					boost::asio::post(pool, [&bobby](){
						bobby->calculateDirection();
					});
				}
			}
			pool.join();
		}
		univers_.manageSnakeClientInput();
	}
	{ // Manage Frame
		for (; nextFrame.empty() && univers_.isOpenGame_();) {
			ptr->lock();
			nextFrame = world_->getEventsManager().getEvents<NextFrame>();
			ptr->unlock();
		}
		nextFrame.clear();

		world_->getEventsManager().destroy<NextFrame>();

	}
	{ //Manage game
		ptr->deliverEvents();
		world_->getSystemsManager().getSystem<FollowSystem>().update();
		world_->getSystemsManager().getSystem<JoystickSystem>().update();
		world_->getEventsManager().destroy<JoystickEvent>();
		world_->getSystemsManager().getSystem<MotionSystem>().update();
		world_->getSystemsManager().getSystem<CollisionSystem>().update();
		world_->getSystemsManager().getSystem<FoodCreationSystem>().update();
		world_->getEventsManager().destroy<FoodCreation>();
		world_->getSystemsManager().getSystem<SpriteSystem>().update();
		world_->getSystemsManager().getSystem<RenderSystem>().update();
		world_->getSystemsManager().getSystem<FoodEatSystem>().update();
		world_->getEventsManager().destroy<FoodEat>();
		world_->update();
	}


}

void GameManager::loopUI() {

	SnakeClient::boost_shared_ptr ptr(univers_.getSnakeClient().lock());

	while (univers_.isOpenGame_() && univers_.displayIsAvailable()) {

		if (!ptr || ptr->allSnakeIsDead())
			break;

		if (univers_.isSwitchLib())
			univers_.manageSwitchLibrary();

		univers_.updateDisplayUI();
	}

	log_error("%s finish", __PRETTY_FUNCTION__);
}

void GameManager::refreshTimerLoopWorld() {
	timer_loop.expires_at(boost::posix_time::microsec_clock::universal_time());
}

void GameManager::finishGame() {
	threadWorldLoop_.join();
	nextFrame.clear();
	world_ = nullptr;
}

const std::shared_ptr<KINU::World> &GameManager::getWorld_() {
	return world_;
}