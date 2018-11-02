#include <utility>


#include <iostream>
#include <KNU/entities/Entity.hpp>
#include <boost/thread/thread.hpp>

#include <KNU/World.hpp>
#include <component/PositionComponent.hpp>
#include <component/MotionComponent.hpp>
#include <component/SpriteComponent.hpp>
#include <systems/MotionSystem.hpp>


#include <logger.h>
#include <component/JoystickComponent.hpp>
#include <events/JoystickEvent.hpp>
#include <systems/JoystickSystem.hpp>
#include <random>
#include <component/FollowComponent.hpp>
#include <systems/FollowSystem.hpp>
#include <component/CollisionComponent.hpp>
#include <systems/CollisionSystem.hpp>
#include <systems/FoodSystem.hpp>
#include <json/forwards.h>
#include <json/json.h>
#include <fstream>
#include <network/Network.hpp>
#include <boost/asio/io_service.hpp>
#include <network/ServerTCP.hpp>
#include <network/ClientTCP.hpp>
#include <future>

#include <gui/Core.hpp>


void init(KNU::World &world) {
	KNU::Entity *snake_follow = nullptr;
	KNU::Entity *new_snake = nullptr;
	for (int index = 0; index < 4; ++index) {
		new_snake = &world.createEntity();
		if (index == 0) {
			new_snake->tag(TAG_HEAD_SNAKE);
			new_snake->addComponent<JoystickComponent>();
			new_snake->addComponent<MotionComponent>();
			new_snake->addComponent<CollisionComponent>();
		}
		new_snake->addComponent<PositionComponent>(7 + index, 10);
		new_snake->group(GROUP_SNAKE);
		if (snake_follow != nullptr)
			new_snake->addComponent<FollowComponent>(snake_follow->getId());
		snake_follow = new_snake;
	}
	if (new_snake)
		new_snake->tag(TAG_TAIL_SNAKE);

	auto &food = world.createEntity();
	food.addComponent<PositionComponent>(7, 8);
	food.addComponent<CollisionComponent>(false);
	food.tag("food");


}

void display(KNU::World &world) {
	for (const auto &entity : world.getEntityManager()
			.getEntitiesByGroup(GROUP_SNAKE)) {
		auto positionComponent = entity.getComponent<PositionComponent>();
		log_info("ID [%d] Position [y:%d|x:%d]", entity.getId(),
				 positionComponent.y, positionComponent.x);
	}
}

int main(int ac, char **av) {
	char path[] = "/tmp/log.out";
	logger_init(path);
	Univers univers;
	univers.create_server();
	univers.create_client();
	univers.getCore_().aState();
	return (0);
}
