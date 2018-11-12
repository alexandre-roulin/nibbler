
#include <component/SpriteComponent.hpp>
#include <logger.h>
#include "Factory.hpp"

const char *Factory::part_name[PART_SNAKE]{
		[HEAD] = "head",
		[BODY] = "body",
		[TAIL] = "tail",
		[GRPS] = "grps"
};

Factory::Factory(KINU::World &world)
		: world_(world) {

}

void Factory::create_all_snake(Snake snake_array[MAX_SNAKE], int16_t nu) {
	int rows = nu / 4;
	int cols = nu % 4;
	rows = !rows ? 1 : rows;
	cols = !cols ? 1 : cols;
	int size_rows = world_.getMax_() / rows;
	int size_cols = world_.getMax_() / cols;

	for (int16_t index = 0; index < nu; ++index) {
		int rel_y = index / 4;
		int rel_x = index % 4;
		int real_y = size_rows * rel_y - size_rows / 2;
		int real_x = size_cols * rel_x - size_cols / 2;
		create_snake(index, real_y, real_x);
	}
	if (nu == 1)
		create_food(8, 8);
	else
		for (int index = 0; index < nu; ++index) {
			create_food(10, 10);
		}
	create_walls();
}

void Factory::create_snake(int16_t id, int y, int x) {
	KINU::Entity snake_follow;
	KINU::Entity new_snake;

	for (int index = 0; index < 4; ++index) {
		new_snake = world_.createEntity();
		log_info("[%d]", new_snake.entitiesManager);
		if (index == 0) {
			new_snake.tag(factory_name(HEAD, id));
			new_snake.addComponent<JoystickComponent>(NORTH,
													   factory_name(HEAD, id));
			new_snake.addComponent<MotionComponent>();
			new_snake.addComponent<CollisionComponent>(false);
			new_snake.addComponent<SpriteComponent>(21);
			new_snake.addComponent<PositionComponent>(10, 10);
		} else if (index == 3) {
			new_snake.tag(factory_name(TAIL, id));
			new_snake.addComponent<CollisionComponent>();
			new_snake.addComponent<SpriteComponent>(23);
			new_snake.addComponent<PositionComponent>(15, 15 + index);
		} else {
			new_snake.addComponent<CollisionComponent>();
			new_snake.addComponent<PositionComponent>(15, 15 + index);
			new_snake.addComponent<SpriteComponent>(22);
		}
		new_snake.group(factory_name(GRPS, id));
		if (snake_follow.isValid())
			new_snake.addComponent<FollowComponent>(snake_follow.getId());
		snake_follow = new_snake;
	}
}

eSnakePart Factory::isSnakePart(std::string compare) {
	for (int index = 0; index < PART_SNAKE; ++index)
		if (std::strstr(compare.c_str(), part_name[index]) != NULL)
			return static_cast<eSnakePart>(index);
	return OTHER;
}

void Factory::create_food(int y, int x) {
	log_trace("Factory::create_food(int y = %d, int x = %d)", y, x);
	auto food = world_.createEntity();
	food.addComponent<PositionComponent>(y, x);
	food.addComponent<CollisionComponent>(false);
	food.addComponent<SpriteComponent>(33);
}

void Factory::create_walls() {
	int max = world_.getMax_();
	int x = 0;
	int y = 1;
	for (; x < max; ++x) {
//		std::cout << "max: " << max << " X:" << x << " Y:" << max - 1 << std::endl;
//		std::cout << "max: " << max << " X:" << x << " Y:" << 0 << std::endl;
		create_wall(x, max - 1);
		create_wall(x, 0);
	}
	for (; y < max - 1; ++y) {
//		std::cout << "max: " << max << " X:" << 0 << " Y:" << y << std::endl;
//		std::cout << "max: " << max << " X:" << max - 1  << " Y:" << y << std::endl;
		create_wall(0, y);
		create_wall(max - 1, y);
	}
}

void Factory::create_wall(int x, int y) {
	auto entity = world_.createEntity();
	entity.addComponent<PositionComponent>(y, x);
	entity.addComponent<CollisionComponent>();
	entity.group(WALL_TAG);
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

std::string Factory::factory_name(eSnakePart esp, int16_t id) {
	static const char BASE_SNAKE_NAME[] = "_snake_";
	//"$ID{1}_snake_[body/tail/head]"
	std::string name;
	name += std::to_string(id);
	name += BASE_SNAKE_NAME;
	name += part_name[esp];
	return name;
}

std::string Factory::factory_name(eSnakePart esp, std::string const s) {
	std::string replace(s);
	replace.replace(s.size() - 4, 4, part_name[esp]);
	return replace;
}
