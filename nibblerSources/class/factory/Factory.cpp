
#include <component/SpriteComponent.hpp>
#include <events/FoodCreation.hpp>
#include <logger.h>
#include <KINU/Entity.hpp>
#include <component/JoystickComponent.hpp>
#include <component/MotionComponent.hpp>
#include <component/CollisionComponent.hpp>
#include <component/FollowComponent.hpp>
#include <cores/Univers.hpp>
#include "Factory.hpp"
#include <KINU/World.hpp>
#include <cores/GameManager.hpp>

Factory::Factory(Univers &univers)
		: univers_(univers) {

}

void Factory::createAllSnake(SnakeArrayContainer &snake_array, int16_t nu) {
	log_info("Create %d snake(s)", nu);

	std::for_each(snake_array.begin(), snake_array.end(),
			[this, nu](Snake const &snake){
		if (snake.isValid) createSnake(snake, nu);
	});
	if (!univers_.isBorderless())
		createWalls();
}

void Factory::createSnake(Snake const &snake, int maxSnakes) {
	KINU::Entity	snake_follow;
	KINU::Entity	new_snake;

	std::shared_ptr<KINU::World> world = univers_.getGameManager().getWorld_();
	// (0 + 1) * 35 / (2 + 1) = 11
	// 35 / 2 = 17

	int base_x = (snake.id + 1) * univers_.getMapSize() / (maxSnakes + 1);
	int base_y = univers_.getMapSize() / 2;
	for (int index = 0; index < 4; ++index) {

		new_snake = world->createEntity();

		if (index == 0) {
			new_snake.tagByTagId(eTag::kHeadTag + snake.id);
//			univers_.getWorld_().getEntitiesManager().addComponent<JoystickComponent>(new_snake, kNorth);
			new_snake.addComponent<JoystickComponent>(kNorth);
			new_snake.addComponent<MotionComponent>(kNorth);
			new_snake.addComponent<CollisionComponent>();
			new_snake.addComponent<SpriteComponent>(eSprite::kHead | snake.sprite, kSpecificLast);
			new_snake.addComponent<PositionComponent>(base_x, base_y);
			//univers_.getGrid_()(base_x, base_y) = eSprite::HEAD | snake.sprite;
			log_warn("Factory::creationHead x[%d] y[%d] id[%d] tag[%d]",base_x, base_y,snake.id,  eTag::kHeadTag + snake.id);
		}
		else if (index == 3) {
			new_snake.tagByTagId(eTag::kTailTag + snake.id);
			new_snake.addComponent<FollowComponent>(snake_follow.getId(), false);
			new_snake.addComponent<CollisionComponent>();
			new_snake.addComponent<SpriteComponent>(eSprite::kTail | snake.sprite, kSpecificLast);
			new_snake.addComponent<PositionComponent>(base_x + 1, base_y);
			//univers_.getGrid_()(base_x + 1, base_y) = eSprite::TAIL | snake.sprite;
			log_warn("Factory::creationTail x[%d] y[%d] id[%d] tag[%d]",base_x + 1, base_y,snake.id, eTag::kTailTag + snake.id);
		}
		else {
			new_snake.addComponent<FollowComponent>(snake_follow.getId(), false);
			new_snake.addComponent<CollisionComponent>();
			new_snake.addComponent<PositionComponent>(base_x + (index - 1), base_y + 1);
			//univers_.getGrid_()(base_x + (index - 1), base_y + 1) = eSprite::BODY | snake.sprite;
			new_snake.addComponent<SpriteComponent>(eSprite::kBody | snake.sprite, kMinorPriority);
			log_warn("Factory::creationBody x[%d] y[%d]",base_x + (index - 1), base_y + 1);
		}
		new_snake.groupEntityByGroupId(snake.id);
		snake_follow = new_snake;
	}

}


void Factory::createWalls() {
	int max = univers_.getMapSize();
//	std::cout << "max > " << max << std::endl;
	int x = 0;
	int y = 1;
	for (; x < max; ++x) {
		createWall(x, 0);
		createWall(x, max - 1);
	}
	for (; y < max - 1; ++y) {
		createWall(0, y);
		createWall(max - 1, y);
	}
}

void Factory::createWall(int x, int y) {

	KINU::Entity entity = univers_.getGameManager().getWorld_()->createEntity();
	univers_.getGrid_()(x, y) = eSprite::kWall;
	entity.addComponent<PositionComponent>(x, y);
	entity.addComponent<CollisionComponent>();
	entity.addComponent<SpriteComponent>(eSprite::kWall, kNoPriority);

	entity.groupEntityByGroupId(eTag::kWallTag);
}

