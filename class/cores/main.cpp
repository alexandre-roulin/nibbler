#include <utility>


#include <iostream>
#include <KINU/Entity.h>
#include <boost/thread/thread.hpp>

#include <KINU/World.h>


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

bool demoGui(int ac, char **av, Univers &univers) {

	if (ac > 1 && !strcmp(av[1], "demo")) {
		univers.create_ui();
		univers.getCore_().demo();
		return (true);
	}
	return (false);
}

int f(Univers &univers) {
	univers.create_server();
	univers.create_client();
	sleep(1);
	univers.getClientTCP_().change_state_ready();
	univers.start_game();

	sleep(1);
	int16_t rand = 42;
	univers.getClientTCP_()
			.write_socket(ClientTCP::add_prefix(START_GAME, &rand));
	univers.loop();
	return 0x2a;
}

int main(int ac, char **av) {
	char path[] = "/tmp/log.out";
	logger_init(path);
	Univers univers;
	srand(time(NULL));

	if (demoGui(ac, av, univers))
		return (0);
	if (ac == 3)
		f(univers);
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

		if (buffer == "game") {
			univers.start_game();
			int16_t rand = 42;
			univers.getClientTCP_()
			.write_socket(ClientTCP::add_prefix(START_GAME, &rand));
			univers.loop();
		}
		if (buffer == "start") {

		}


		if (buffer == "ui") {
			univers.create_ui();
			univers.getCore_().aState();
			return (0);
		}
	}
}
