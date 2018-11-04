
#include <component/SpriteComponent.hpp>
#include "Factory.hpp"

const char *Factory::part_name[4] {
		[HEAD] = "head",
		[BODY] = "body",
		[TAIL] = "tail",
		[GRP] = "grps"
};

Factory::Factory(KNU::World &world)
		: world_(world) {

}

void Factory::create_all_snake(Snake snake_array[MAX_SNAKE], int16_t nu) {
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

	for (int index = 0; index < 4; ++index) {
		new_snake = &world_.createEntity();
		if (index == 0) {
			new_snake->tag(factory_name(HEAD, id));
			new_snake->addComponent<JoystickComponent>();
			new_snake->addComponent<MotionComponent>();
			new_snake->addComponent<CollisionComponent>(false);
			new_snake->addComponent<SpriteComponent>(1);
		}else {
			new_snake->tag(factory_name(BODY, id));
		}
		new_snake->addComponent<PositionComponent>(y + index, x);
		new_snake->group(factory_name(GRP, id));
		if (snake_follow != nullptr)
			new_snake->addComponent<FollowComponent>(snake_follow->getId());
		snake_follow = new_snake;

	}
	if (new_snake)
		new_snake->tag(factory_name(TAIL, id));
}

eSnakePart Factory::isSnakePart(std::string compare) {
	std::string snake_base("?_snake_????");
	if (snake_base.compare(1, 7, compare) == 0)
		for (int index = 0 ; index < 4; ++index) {
			if (compare.compare(7, 4, part_name[index]) == 0)
				return static_cast<eSnakePart>(index);
		}
	return OTHER;
}

void Factory::create_food(int y, int x) {
	auto &food = world_.createEntity();
	food.addComponent<PositionComponent>(y, x);
	food.addComponent<CollisionComponent>(false);
	food.tag("food");
}

char * Factory::factory_name(eSnakePart esp, int16_t id) {
	static char name[13] = "?_snake_????";
	//"$ID{1}_snake_[body/tail/head]"
	name[0] = id + 48;
	std::memcpy(name + 8, part_name[esp], sizeof(char) * 4);
	return name;
}

std::string Factory::factory_name(eSnakePart esp, std::string const s) {
	std::string replace(s);
	replace.replace(8, 4, part_name[esp]);
	return s;
}
