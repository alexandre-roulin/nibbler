
#include <component/SpriteComponent.hpp>
#include <events/FoodCreation.hpp>
#include <logger.h>
#include <KINU/Entity.hpp>
#include <component/JoystickComponent.hpp>
#include <component/MotionComponent.hpp>
#include <component/CollisionComponent.hpp>
#include <component/FollowComponent.hpp>
#include <Univers.hpp>
#include "Factory.hpp"
#include <KINU/World.hpp>

Factory::Factory(Univers &univers)
		: univers_(univers) {

}

void Factory::create_all_snake(std::array<Snake, SNAKE_MAX> snake_array, int16_t nu) {
	log_info("Create %d snake(s)", nu);

	std::for_each(snake_array.begin(), snake_array.end(),
			[this, nu](Snake const &snake){
		if (snake.id != -1) create_snake(snake,nu);
	});
	if (!univers_.isBorderless())
		create_walls();
}

void Factory::create_snake(Snake const &snake, int max_snakes) {
	KINU::Entity	snake_follow;
	KINU::Entity	new_snake;

	// (0 + 1) * 35 / (2 + 1) = 11
	// 35 / 2 = 17

	int base_x = (snake.id + 1) * univers_.getMapSize() / (max_snakes + 1);
	int base_y = univers_.getMapSize() / 2;
	for (int index = 0; index < 4; ++index) {

		new_snake = univers_.getWorld_().createEntity();

		if (index == 0) {
			new_snake.tagByTagId(eTag::HEAD_TAG + snake.id);
//			univers_.getWorld_().getEntitiesManager().addComponent<JoystickComponent>(new_snake, kNorth);
			new_snake.addComponent<JoystickComponent>(kNorth);
			new_snake.addComponent<MotionComponent>(kNorth);
			new_snake.addComponent<CollisionComponent>();
			new_snake.addComponent<SpriteComponent>(eSprite::HEAD | snake.sprite, SPECIFIC_LAST);
			new_snake.addComponent<PositionComponent>(base_x, base_y);
			univers_.getGrid_()(base_x, base_y) = eSprite::HEAD | snake.sprite;
			log_warn("Factory::creationHead x[%d] y[%d] id[%d] tag[%d]",base_x, base_y,snake.id,  eTag::HEAD_TAG + snake.id);
		}
		else if (index == 3) {
			new_snake.tagByTagId(eTag::TAIL_TAG + snake.id);
			new_snake.addComponent<FollowComponent>(snake_follow.getId(), false);
			new_snake.addComponent<CollisionComponent>();
			new_snake.addComponent<SpriteComponent>(eSprite::TAIL | snake.sprite, SPECIFIC_LAST);
			new_snake.addComponent<PositionComponent>(base_x + 1, base_y);
			univers_.getGrid_()(base_x + 1, base_y) = eSprite::TAIL | snake.sprite;
			log_warn("Factory::creationTail x[%d] y[%d] id[%d] tag[%d]",base_x + 1, base_y,snake.id, eTag::TAIL_TAG + snake.id);
		}
		else {
			new_snake.addComponent<FollowComponent>(snake_follow.getId(), false);
			new_snake.addComponent<CollisionComponent>();
			new_snake.addComponent<PositionComponent>(base_x + (index - 1), base_y + 1);
			univers_.getGrid_()(base_x + (index - 1), base_y + 1) = eSprite::BODY | snake.sprite;
			new_snake.addComponent<SpriteComponent>(eSprite::BODY | snake.sprite, MINOR_PRIORITY);
			log_warn("Factory::creationBody x[%d] y[%d]",base_x + (index - 1), base_y + 1);
		}
		new_snake.groupEntityByGroupId(snake.id);
		snake_follow = new_snake;
	}

}


void Factory::create_walls() {
	int max = univers_.getMapSize();
//	std::cout << "max > " << max << std::endl;
	int x = 0;
	int y = 1;
	for (; x < max; ++x) {
		create_wall(x, 0);
		create_wall(x, max - 1);
	}
	for (; y < max - 1; ++y) {
		create_wall(0, y);
		create_wall(max - 1, y);
	}
}

void Factory::create_wall(int x, int y) {
//	std::cout << x << " " << y << std::endl;
	KINU::Entity entity = univers_.getWorld_().createEntity();
	univers_.getGrid_()(x, y) = eSprite::WALL;
	entity.addComponent<PositionComponent>(x, y);
	entity.addComponent<CollisionComponent>();
	entity.addComponent<SpriteComponent>(eSprite::WALL, NO_PRIORITY);

	entity.groupEntityByGroupId(eTag::WALL_TAG);
}

