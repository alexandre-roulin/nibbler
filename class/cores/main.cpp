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

void f(boost::asio::io_service &io){

}
int main(int ac, char **av) {
	std::string buffer;
	boost::asio::io_service io_server;
	boost::asio::io_service io_client;

	try {
		for (;;) {
			std::cout << "$> ";
			std::getline(std::cin, buffer);
			if (buffer == "server") {
				new ServerTCP(io_server);
				boost::thread t(boost::bind(&boost::asio::io_service::run, &io_server));
				t.join();
			}
			if (buffer == "client") {
				std::cout << "Hostname : ";
				std::getline(std::cin, buffer);
				new ClientTCP(io_client, buffer);
				boost::thread t(boost::bind(&boost::asio::io_service::run, &io_client));
				t.join();
			}

		}
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
//	Network net;
//	std::string buffer;
//	for (;;) {
//		std::cout << "New command : ";
//		std::getline(std::cin, buffer);
//		if (buffer == "connect")
//			net.connect_socket();
//		if (buffer == "accept")
//			net.accept_socket();
//		if (buffer == "sendall")
//			net.send_socket();
//		if (buffer == "recv")
//			net.recv_socket();
//	}
//	return 1;
	char path[] = "/tmp/log.out";
	logger_init(path);

	KNU::World world;

	world.getSystemManager().addSystem<MotionSystem>();
	world.getSystemManager().addSystem<JoystickSystem>();
	world.getSystemManager().addSystem<FollowSystem>();
	world.getSystemManager().addSystem<CollisionSystem>();
	world.getSystemManager().addSystem<FoodSystem>();

	std::ifstream file("./save.json");
	Json::Value input;
	file >> input;
	world.getEntityManager().deserializeKNU(input);

	display(world);
	for (int index = 0; index < 2; ++index) {
		world.update();                                                         //WORLD			-> Distribute/Destroy entity to System/EntityManager
		world.getEventManager().emitEvent<JoystickEvent>(ARROW_LEFT);

		world.getSystemManager().getSystem<FollowSystem>()->update();           //FOLLOW		-> Save Next position
		world.getSystemManager().getSystem<JoystickSystem>()->update();         //JOYSTICK		-> Move Joystick 's position component
		world.getSystemManager().getSystem<MotionSystem>()->update();           //MOTION		-> Move Motion Component
		world.getSystemManager().getSystem<CollisionSystem>()->update();        //COLLISION		-> Check Collision with CollisionComponent
		world.getSystemManager().getSystem<FoodSystem>()->update();             //FOOD			-> Check FoodEvent and add Snake if needed
		display(world);
	}

	for (int index = 0; index < 2; ++index) {
		world.update();
		world.getEventManager().emitEvent<JoystickEvent>(ARROW_UP);
		world.getSystemManager().getSystem<FollowSystem>()->update();
		world.getSystemManager().getSystem<JoystickSystem>()->update();
		world.getSystemManager().getSystem<MotionSystem>()->update();
		world.getSystemManager().getSystem<CollisionSystem>()->update();
		world.getSystemManager().getSystem<FoodSystem>()->update();
		display(world);
	}
}

//