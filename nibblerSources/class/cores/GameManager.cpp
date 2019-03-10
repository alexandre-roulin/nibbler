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

const uint32_t GameManager::Easy = 150000;  //0.15 sec frame
const uint32_t GameManager::Medium = 115000;  //0.115sec frame
const uint32_t GameManager::Hard = 80000;  //0.08sec frame
const uint32_t GameManager::Impossible = 10000;  //0.01sec frame

GameManager::GameManager(Univers &univers)
	:
	univers_(univers),
	timer_loop(univers_.getIoManager().getIo()) {
}


void GameManager::startNewGame() {
	std::cout << "Game Init" << std::endl;
	world_ = std::make_unique<KINU::World>();

	univers_.setGrid_(std::make_shared<MutantGrid<eSprite>>(univers_.getMapSize()));
	univers_.getGrid_().fill(eSprite::kNone);

	world_->getSystemsManager().addSystem<CollisionSystem>(univers_);
	world_->getSystemsManager().addSystem<FollowSystem>();
	world_->getSystemsManager().addSystem<JoystickSystem>();
	world_->getSystemsManager().addSystem<MotionSystem>(univers_);
	world_->getSystemsManager().addSystem<SpriteSystem>(univers_);
	world_->getSystemsManager().addSystem<RenderSystem>(univers_);
	world_->getSystemsManager().addSystem<FoodCreationSystem>(univers_.getMapSize());
	world_->getSystemsManager().addSystem<FoodEatSystem>();
	world_->getEventsManager().emitEvent(NextFrame());
	world_->getEventsManager().destroy<NextFrame>();

	if (univers_.isServer()) {
		univers_.getSnakeServer().startGame();
	}

	SnakeClient::boost_shared_ptr ptr(univers_.getSnakeClient().lock());

	std::vector<StartEvent> startEvent;
	std::cout << "Wait Event Init" << std::endl;

	for (;ptr && startEvent.empty();) {
		ptr->lock();
		startEvent = world_->getEventsManager().getEvents<StartEvent>();
		ptr->unlock();
		if (!univers_.isOpenGame_())
			return ;
	}
	std::cout << "Post Event Init" << std::endl;

	world_->update();
	world_->getSystemsManager().getSystem<SpriteSystem>().update();
	world_->getSystemsManager().getSystem<RenderSystem>().update();
	univers_.updateDisplayUI();

	if (univers_.isServer()) {
		for (auto &bobby : univers_.getBobbys()) {
			bobby->buildIA();
		}
	}

	std::cout << "Thread Init" << std::endl;

	threadWorldLoop_ = boost::thread([this, startEvent](){

		boost::asio::deadline_timer timer_start(univers_.getIoManager().getIo());
		assert(!startEvent.empty());
		timer_start.expires_at(startEvent.front().start_time);
		timer_start.wait();

		loopGame();
	});
	std::cout << "Thread Init" << std::endl;
}


void GameManager::loopGame() {
	SnakeClient::boost_shared_ptr ptr(univers_.getSnakeClient().lock());

	while (!ptr->allSnakeIsDead() && univers_.isOpenGame_() && univers_.displayIsAvailable()) {
		manageGlobalInput();
		loopWorld();
		timer_loop.expires_from_now(boost::posix_time::microsec(univers_.getMicroSecDeltaTime()));
		timer_loop.wait();
	}
}

void GameManager::manageGlobalInput() {
	boost::asio::thread_pool pool(8);
	SnakeClient::boost_shared_ptr ptr(univers_.getSnakeClient().lock());
	if (!ptr)
		return;
	std::cout << " /** Manage Input **/" << std::endl;
	{ /** Manage Input **/
		Bobby::clearPriority();
		if (univers_.isServer()) {
			for (auto &bobby : univers_.getBobbys()) {
				if (world_->getEntitiesManager().hasEntityByTagId(bobby->getId() + eTag::kHeadTag)) {
					boost::asio::post(pool, [&bobby](){ bobby->calculateDirection(); });
				}
			}
			pool.join();
		}
		univers_.manageSnakeClientInput();
	}
	std::cout << " /** Manage Frame **/" << std::endl;
	{ /** Manage Frame **/
		for (; world_->getEventsManager().getEvents<NextFrame>().empty() && univers_.isOpenGame_(););
		ptr->lock();
		world_->getEventsManager().destroy<NextFrame>();

		DirectionArray directions;
		size_t n = 0;
		SnakeArrayContainer array = *(ptr->getSnakeArray_());
		assert(array.size() == directions.size());
		assert(array.size() == SNAKE_MAX);
		std::generate(directions.begin(), directions.end(), [&n, array]{ return array[n++].direction;});
		world_->getEventsManager().emitEvent<DirectionArray>(directions);
		for (int index = 0; index < SNAKE_MAX; ++index) {
			assert(array[index].direction == directions[index]);
		}
		ptr->unlock();
	}
}

void GameManager::loopWorld() {

	SnakeClient::boost_shared_ptr ptr(univers_.getSnakeClient().lock());
	if (!ptr) return;
	std::cout << "/** Manage game **/" << std::endl;

	{ /** Manage game **/
		ptr->deliverEvents();
		world_->getSystemsManager().getSystem<FollowSystem>().update();
//		std::cout << "FollowSystem" << std::endl;
		world_->getSystemsManager().getSystem<JoystickSystem>().update();
//		std::cout << "JoystickSystem" << std::endl;
		world_->getEventsManager().destroy<JoystickEvent>();
//		std::cout << "JoystickEvent" << std::endl;
		world_->getSystemsManager().getSystem<MotionSystem>().update();
//		std::cout << "MotionSystem" << std::endl;
		world_->getSystemsManager().getSystem<CollisionSystem>().update();
//		std::cout << "CollisionSystem" << std::endl;
		world_->getSystemsManager().getSystem<FoodCreationSystem>().update();
//		std::cout << "FoodCreationSystem" << std::endl;
		world_->getEventsManager().destroy<FoodCreation>();
//		std::cout << "FoodCreation" << std::endl;
		world_->getSystemsManager().getSystem<SpriteSystem>().update();
//		std::cout << "SpriteSystem" << std::endl;
		world_->getSystemsManager().getSystem<RenderSystem>().update();
//		std::cout << "RenderSystem" << std::endl;
		world_->getSystemsManager().getSystem<FoodEatSystem>().update();
//		std::cout << "FoodEatSystem" << std::endl;
		world_->getEventsManager().destroy<FoodEat>();
//		std::cout << "FoodEat" << std::endl;
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

}

void GameManager::refreshTimerLoopWorld() {
	timer_loop.expires_at(boost::posix_time::microsec_clock::universal_time());
}

void GameManager::finishGame() {
	threadWorldLoop_.join();
	world_ = nullptr;
}

const std::shared_ptr<KINU::World> &GameManager::getWorld_() {
	return world_;
}
