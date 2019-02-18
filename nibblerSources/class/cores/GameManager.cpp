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

GameManager::GameManager(Univers &univers)
	: univers_(univers),
	timer_loop(univers_.getIoManager().getIo()){
}


void GameManager::startNewGame() {

	world_ = std::make_unique<KINU::World>();

	univers_.setGrid_(std::make_shared<MutantGrid<eSprite>>(univers_.getMapSize()));
	univers_.getGrid_().fill(eSprite::kNone);


	if (univers_.isServer()) {
		univers_.getSnakeServer().startGame();
	}

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

	for (;ptr && startEvent.empty();) {
		ptr->lock();
		startEvent = world_->getEventsManager().getEvents<StartEvent>();
		ptr->unlock();
	}

	timer_start.expires_at(startEvent.front().start_time);
	timer_start.wait();

	if (univers_.isServer()) {
		for (auto &bobby : univers_.getBobbys()) {
			bobby->buildIA();
			bobby->sendDirection();
		}
	}
	world_->update();
	world_->getSystemsManager().getSystem<SpriteSystem>().update();
	world_->getSystemsManager().getSystem<RenderSystem>().update();

	timer_loop.expires_from_now(boost::posix_time::milliseconds(univers_.getGameSpeed()));
	timer_loop.async_wait(boost::bind(&GameManager::loopWorld, this));
}

void GameManager::loopWorld() {
	SnakeClient::boost_shared_ptr ptr(univers_.getSnakeClient().lock());
	if (!univers_.isOpenGame_() || !ptr)
		return;

	univers_.manageSnakeClientInput();
	threadGroup.join_all();
	for (; nextFrame.empty() && univers_.isOpenGame_() && world_ && ptr;) {
		ptr->lock();
		nextFrame = world_->getEventsManager().getEvents<NextFrame>();
		ptr->unlock();
	}

	nextFrame.clear();
	world_->getEventsManager().destroy<NextFrame>();

	ptr->deliverEvents();

	world_->update();

	world_->getSystemsManager().getSystem<FollowSystem>().update();
	world_->getSystemsManager().getSystem<JoystickSystem>().update();
	world_->getEventsManager().destroy<JoystickEvent>();
	world_->getSystemsManager().getSystem<MotionSystem>().update();
	world_->getSystemsManager().getSystem<CollisionSystem>().update();
	world_->getSystemsManager().getSystem<FoodCreationSystem>().update();
	world_->getEventsManager().destroy<FoodCreation>();
	world_->getSystemsManager().getSystem<SpriteSystem>().update();
	world_->getSystemsManager().getSystem<RenderSystem>().update();

	Bobby::clearPriority();
	if (univers_.isServer()) {
		for (auto &bobby : univers_.getBobbys()) {
			if (world_->getEntitiesManager().hasEntityByTagId(bobby->getId() + eTag::kHeadTag)) {
				threadGroup.create_thread(boost::bind(&Bobby::calculateDirection, bobby.get()));
			}

		}
	}


	world_->getSystemsManager().getSystem<FoodEatSystem>().update();
	world_->getEventsManager().destroy<FoodEat>();

	if (!ptr->allSnakeIsDead()) {

		timer_loop.expires_at(
				timer_loop.expires_at() +
				boost::posix_time::milliseconds(univers_.getGameSpeed()));
		timer_loop.async_wait(boost::bind(&GameManager::loopWorld, this));
	}
}

void GameManager::loopUI() {

	SnakeClient::boost_shared_ptr ptr(univers_.getSnakeClient().lock());

	while (univers_.getGameSpeed() && univers_.displayIsAvailable()) {

		if (!ptr || ptr->allSnakeIsDead())
			break;

		if (univers_.isSwitchLib())
			univers_.manageSwitchLibrary();

		univers_.updateDisplayUI();
	}
}

void GameManager::refreshTimerLoopWorld() {
	timer_loop.expires_at(boost::posix_time::microsec_clock::universal_time());
}


KINU::World &GameManager::getWorld_() {
	return *world_;
}

void GameManager::finishGame() {
	timer_loop.wait();
	timer_loop.cancel();
	nextFrame.clear();
	world_ = nullptr;
}
