
#include <component/SpriteComponent.hpp>
#include <component/PreviousComponent.hpp>
#include <events/FoodCreation.hpp>
#include "Factory.hpp"

const char *Factory::part_name[PART_SNAKE]{
		[HEAD] = "head",
		[BODY] = "body",
		[TAIL] = "tail",
		[GRPS] = "grps"
};

Factory::Factory(Univers &univers)
		: univers_(univers) {

}

void Factory::create_all_snake(Snake snake_array[MAX_SNAKE], int16_t nu) {

	log_info("Create %d snake(s)", nu);
	for (int index = 0; index < nu; ++index) {
		create_snake(snake_array[index], nu);
	}
	for (int index = 0; index < nu; ++index) {
		univers_.getWorld_().getEventManager().emitEvent<FoodCreation>(10, 10 +
																		   index);
	}
	create_walls();
}

void Factory::create_snake(Snake snake, int max_snakes) {
	KINU::Entity	snake_follow;
	KINU::Entity	new_snake;
	bool			firstSnake = true;

	int base_x = (snake.id + 1) * univers_.getMapSize() / (max_snakes + 1);
	int base_y = univers_.getMapSize() / 2;
	for (int index = 0; index < 4; ++index) {
		new_snake = univers_.getWorld_().createEntity();
		if (!firstSnake && snake_follow.hasComponent<PreviousComponent>())
			snake_follow.getComponent<PreviousComponent>().idPrevious = new_snake.getIndex();
		if (index == 0) {
			new_snake.tag(factory_name(HEAD, snake.id));
			new_snake.addComponent<JoystickComponent>(NORTH);
			new_snake.addComponent<MotionComponent>(NORTH);
			new_snake.addComponent<CollisionComponent>();
			new_snake.addComponent<SpriteComponent>(eSprite::HEAD | snake.sprite);
			new_snake.addComponent<PositionComponent>(base_x, base_y);
		}
		else if (index == 3) {
			new_snake.tag(factory_name(TAIL, snake.id));
			new_snake.addComponent<FollowComponent>(snake_follow.getIndex(), false);
			new_snake.addComponent<CollisionComponent>();
			new_snake.addComponent<SpriteComponent>(eSprite::TAIL | snake.sprite);
			new_snake.addComponent<PositionComponent>(15, 15 + index);
		}
		else {
			new_snake.addComponent<FollowComponent>(snake_follow.getIndex(), false);
			new_snake.addComponent<CollisionComponent>();
			new_snake.addComponent<PreviousComponent>(0, false);
			new_snake.addComponent<PositionComponent>(15, 15 + index);
			new_snake.addComponent<SpriteComponent>(eSprite::BODY | snake.sprite);
		}
		new_snake.group(factory_name(GRPS, snake.id));
		snake_follow = new_snake;
		firstSnake = false;
	}
}

eSnakePart Factory::isSnakePart(std::string compare) {
	for (int index = 0; index < PART_SNAKE; ++index)
		if (std::strstr(compare.c_str(), part_name[index]) != NULL)
			return static_cast<eSnakePart>(index);
	return OTHER;
}

void Factory::create_walls() {
	int max = 30;
	int x = 0;
	int y = 1;
	for (; x < max; ++x) {
		create_wall(x, max - 1);
		create_wall(x, 0);
	}
	for (; y < max - 1; ++y) {
		create_wall(0, y);
		create_wall(max - 1, y);
	}
}

void Factory::create_wall(int x, int y) {
	auto entity = univers_.getWorld_().createEntity();
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

KINU::Entity::Id Factory::getIdFromTag(std::string tag) {
	return std::stoi(tag);
}
