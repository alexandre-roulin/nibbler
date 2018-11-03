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
	std::string buffer;
	for (;;) {
		std::cout << "$> ";
		std::getline(std::cin, buffer);
		if (buffer == "server")
			univers.create_server();
		if (buffer == "client")
			univers.create_client();
		if (buffer == "start") {
			univers.create_ui();
			univers.getCore_().aState();
			return (0);
		}
	}
}
