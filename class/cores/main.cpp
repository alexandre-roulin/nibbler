#include <utility>


#include <iostream>
#include <KNU/entities/Entity.hpp>
#include <boost/thread/thread.hpp>

#include <KNU/World.hpp>
#include <component/PositionComponent.hpp>
#include <component/MotionComponent.hpp>
#include <component/SpriteComponent.hpp>
#include <logger.h>


#include <component/JoystickComponent.hpp>
#include <events/JoystickEvent.hpp>

#include <random>
#include <component/FollowComponent.hpp>
#include <component/CollisionComponent.hpp>
#include <json/forwards.h>
#include <json/json.h>
#include <fstream>
#include <network/Network.hpp>
#include <boost/asio/io_service.hpp>
#include <network/ServerTCP.hpp>
#include <network/ClientTCP.hpp>
#include <future>

#include <gui/Core.hpp>

std::string const Snake::basicName[MAX_SNAKE] = { "Jack O'Lantern", "Eden", "Jacky", "Emerald", "Broutille", "Veggie-vie", "jinou42", "Dota c cro cool" };

bool	demoGui(int ac, char **av, Univers &univers)
{

	if (ac > 1 && !strcmp(av[1], "demo"))
	{
		univers.create_ui();
		univers.getCore_().demo();
		return (true);
	}
	return (false);
}

int main(int ac, char **av) {
	char		path[] = "/tmp/log.out";
	logger_init(path);
	Univers		univers;

	if (demoGui(ac, av, univers))
		return (0);

	std::string buffer;
	for (;;) {
		std::cout << "$> ";
		std::getline(std::cin, buffer);
		if (buffer == "server")
			univers.create_server();
		if (buffer == "client")
			univers.create_client();
		if (buffer == "ready") {
			univers.getClientTCP_().change_state_ready();
		}
		if (buffer == "food")
			univers.getClientTCP_().food();
		if (buffer == "start") {
//			univers.create_ui();
//			univers.getCore_().aState();
			univers.start_game();
			return (0);
		}
	}
}
