
#include <component/SpriteComponent.hpp>
#include <events/FoodCreation.hpp>
#include <logger.h>
#include "Factory.hpp"

Factory::Factory(Univers &univers)
		: univers_(univers) {

}

void Factory::create_all_snake(Snake snake_array[MAX_SNAKE], int16_t nu) {

	log_info("Create %d snake(s)", nu);
	for (int index = 0; index < nu; ++index) {
		create_snake(snake_array[index], nu);
	}
	if (!univers_.isBorderless())
		create_walls();
}

void Factory::create_snake(Snake snake, int max_snakes) {
	KINU::Entity	snake_follow;
	KINU::Entity	new_snake;

	// (0 + 1) * 35 / (2 + 1) = 11
	// 35 / 2 = 17

	int base_y = (snake.id + 1) * univers_.getMapSize() / (max_snakes + 1);
	int base_x = univers_.getMapSize() / 2;
	for (int index = 0; index < 4; ++index) {

		new_snake = univers_.getWorld_().createEntity();

		if (index == 0) {
			new_snake.tagByTagId(eTag::HEAD_TAG + snake.id);
			new_snake.addComponent<JoystickComponent>(NORTH);
			new_snake.addComponent<MotionComponent>(NORTH);
			new_snake.addComponent<CollisionComponent>();
			new_snake.addComponent<SpriteComponent>(eSprite::HEAD | snake.sprite, SPECIFIC_LAST);
			new_snake.addComponent<PositionComponent>(base_x, base_y);
			log_warn("Factory::creationHead x[%d] y[%d] id[%d] tag[%d]",base_x, base_y,snake.id,  eTag::HEAD_TAG + snake.id);
		}
		else if (index == 3) {
			new_snake.tagByTagId(eTag::TAIL_TAG + snake.id);
			new_snake.addComponent<FollowComponent>(snake_follow.getId(), false);
			new_snake.addComponent<CollisionComponent>();
			new_snake.addComponent<SpriteComponent>(eSprite::TAIL | snake.sprite, SPECIFIC_LAST);
			new_snake.addComponent<PositionComponent>(base_x + 1, base_y);
			log_warn("Factory::creationTail x[%d] y[%d] id[%d] tag[%d]",base_x + 1, base_y,snake.id, eTag::TAIL_TAG + snake.id);
		}
		else {
			new_snake.addComponent<FollowComponent>(snake_follow.getId(), false);
			new_snake.addComponent<CollisionComponent>();
			new_snake.addComponent<PositionComponent>(base_x + (index - 1), base_y + 1);
			new_snake.addComponent<SpriteComponent>(eSprite::BODY | snake.sprite, MINOR_PRIORITY);
			log_warn("Factory::creationBody x[%d] y[%d]",base_x + (index - 1), base_y + 1);
		}
		new_snake.groupEntityByGroupId(snake.id);
		snake_follow = new_snake;
	}

}


void Factory::create_walls() {
	int max = univers_.getMapSize();
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
	auto entity = univers_.getWorld_().createEntity();
	entity.addComponent<PositionComponent>(x, y);
	entity.addComponent<CollisionComponent>();
	entity.addComponent<SpriteComponent>(eSprite::WALL, NO_PRIORITY);

	entity.groupEntityByGroupId(eTag::WALL_TAG);
}

char *Factory::factory_chat_message(char const *name, char const *message) {
	static char final_[SIZEOF_CHAT_PCKT] = {0};
	bzero(final_, SIZEOF_CHAT_PCKT);
	size_t lenName = std::strlen(name);
	if (lenName > NAME_BUFFER)
		lenName = NAME_BUFFER;
	final_[0] = '[';
	std::memcpy(final_ + 1, name, lenName);
	std::strncpy(final_ + lenName + 1, "]: ", 3);
	size_t lenMessage = strlen(message);
	if (lenMessage > CHAT_BUFFER)
		lenMessage = CHAT_BUFFER;
	std::memcpy(final_ + lenName + 4, message, lenMessage);
	return final_;
}