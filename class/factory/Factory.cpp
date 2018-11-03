
#include "Factory.hpp"

Factory::Factory(KNU::World &world)
		: world_(world) {

}

void Factory::create_all_snake(int16_t nu) {
	int rows = nu / 4;
	int cols = nu % 4;
	int size_rows = world_.getMax_() / rows;
	int size_cols = world_.getMax_() / cols;

	for (int16_t index = 0; index < nu; ++index) {
		int rel_y = index / 4;
		int rel_x = index % 4;
		int real_y = size_rows * rel_y - size_rows / 2;
		int real_x = size_cols * rel_x - size_cols / 2;
		create_snake(index, real_y, real_x);
	}
}

void Factory::create_snake(int16_t id, int y, int x) {
	KNU::Entity *snake_follow = nullptr;
	KNU::Entity *new_snake = nullptr;
	std::string group(GROUP_SNAKE);
	group += id;
	for (int index = 0; index < 4; ++index) {
		new_snake = &world_.createEntity();
		if (index == 0) {
			std::string head(TAG_HEAD_SNAKE);
			head += id;
			new_snake->tag(head);
			new_snake->addComponent<JoystickComponent>();
			new_snake->addComponent<MotionComponent>();
			new_snake->addComponent<CollisionComponent>();
		}
		new_snake->addComponent<PositionComponent>(y + index, x);
		new_snake->group(group);
		if (snake_follow != nullptr)
			new_snake->addComponent<FollowComponent>(snake_follow->getId());
		snake_follow = new_snake;

	}
	std::string tail(TAG_TAIL_SNAKE);
	tail += id;
	if (new_snake)
		new_snake->tag(tail);
}

void Factory::create_food(int y, int x) {
	auto &food = world_.createEntity();
	food.addComponent<PositionComponent>(y, x);
	food.addComponent<CollisionComponent>(false);
	food.tag("food");
}