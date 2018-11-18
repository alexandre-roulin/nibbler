#include <utility>


#include <iostream>
#include <KINU/Entity.hpp>
#include <boost/thread/thread.hpp>

#include <KINU/World.hpp>


#include <random>
#include <fstream>
#include <network/ServerTCP.hpp>
#include <network/ClientTCP.hpp>
#include <logger.h>
#include <gui/Core.hpp>

std::string const Snake::basicName[MAX_SNAKE] = {"Jack O'Lantern", "Eden",
												 "Jacky", "Emerald",
												 "Broutille", "Veggie-vie",
												 "jinou42", "Dota c cro cool"};

std::ostream &operator<<(std::ostream &os, const Snake &snake) {
	os << "name: " << snake.name << " sprite: " << static_cast<int>(snake.sprite) << " isReady: "
	   << snake.isReady << " id: " << snake.id;
	return os;
}

bool demoGui(int ac, char **av, Univers &univers) {

	if (ac > 1 && !strcmp(av[1], "demo")) {
		univers.create_ui();
		univers.getCore_().demo();
		return (true);
	}
	return (false);
}


int main(int ac, char **av) {
	char path[] = "/tmp/log.out";
	logger_init(path);
	Univers univers;
	srand(time(NULL));

	if (demoGui(ac, av, univers))
		return (0);
	std::string buffer;
	for (;;) {
		std::cout << "$> ";
		std::getline(std::cin, buffer);
		if (buffer == "server")
			univers.create_server();
		if (buffer == "connect") {
			std::string dns, port;
			std::cout << "dns > ";
			std::getline(std::cin, dns);
			std::cout << "port > ";
			std::getline(std::cin, port);
			univers.getClientTCP_().connect(dns, port);
		}
		if (buffer == "ready") {
			univers.getClientTCP_().change_state_ready();
		}

		if (buffer == "game") {
			univers.setMapSize(60);
			univers.load_external_library(std::string("Game pro"),std::string(LIBRARY_SFML));

			ClientTCP::StartInfo startInfo;
			univers.getClientTCP_()
					.write_socket(ClientTCP::add_prefix(START_GAME, &startInfo));
			univers.loop();
		}
		if (buffer == "game1") {
			univers.load_external_library(std::string("Game pro"), std::string(LIBRARY_SDL));
			univers.loop();
		}
		if (buffer == "ui") {
			univers.create_ui();
			univers.getCore_().aState();
			return (0);
		}
	}
}
