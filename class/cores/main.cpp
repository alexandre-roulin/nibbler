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

bool demoGui(int ac, char **av, Univers &univers) {

	if (ac > 1 && !strcmp(av[1], "demo")) {
		univers.create_ui();
		univers.getCore_().demo();
		return (true);
	}
	return (false);
}

bool uiTest(int ac, char **av, Univers &univers) {

	if (ac > 1 && !strcmp(av[1], "test")) {
		univers.create_ui();
		univers.getCore_().mainMenu();
		return (true);
	}
	return (false);
}

int f(Univers &univers) {
	univers.create_server();
	univers.create_client();
	sleep(1);
	univers.getClientTCP_().change_state_ready();
	sleep(1);
	univers.start_game();

	ClientTCP::StartInfo startInfo;
	univers.getClientTCP_()
			.write_socket(ClientTCP::add_prefix(START_GAME, &startInfo));
	univers.loop();
	return 0x2a;
}

int main(int ac, char **av) {
	char path[] = "/tmp/log.out";
	logger_init(path);
	Univers univers;
	srand(time(NULL));

	if (demoGui(ac, av, univers)
		|| uiTest(ac, av, univers))
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
			univers.start_game();

			ClientTCP::StartInfo startInfo;
			univers.getClientTCP_()
					.write_socket(ClientTCP::add_prefix(START_GAME, &startInfo));
			univers.loop();
		}
		if (buffer == "game1") {
			univers.start_game();
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
