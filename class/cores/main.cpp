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
	os << "name: " << snake.name << " sprite: "
	   << static_cast<int>(snake.sprite) << " isReady: "
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

bool uiTest(int ac, char **av, Univers &univers) {

	if (ac > 1 && !strcmp(av[1], "test")) {
		univers.create_ui();
		univers.getCore_().mainMenu();
		return (true);
	}
	return (false);
}

void cal() {
	printf("%ld\n",1542758821590 -1542758821590);
	printf("%ld\n",1542758822590 -1542758822590);
	printf("%ld\n",1542758823592 -1542758823592);
	printf("%ld\n",1542758824588 -1542758824588);
	printf("%ld\n",1542758825591 -1542758825591);
	printf("%ld\n",1542758826587 -1542758826587);
	printf("%ld\n",1542758827587 -1542758827587);
	printf("%ld\n",1542758828587 -1542758828587);
	printf("%ld\n",1542758829588 -1542758829588);
	printf("%ld\n",1542758830587 -1542758830587);
	printf("%ld\n",1542758831591 -1542758831591);
	printf("%ld\n",1542758832592 -1542758832592);
	printf("%ld\n",1542758833591 -1542758833591);
	printf("%ld\n",1542758834592 -1542758834592);
	printf("%ld\n",1542758835588 -1542758835588);
	printf("%ld\n",1542758836589 -1542758836589);
	printf("%ld\n",1542758837588 -1542758837588);
	printf("%ld\n",1542758838588 -1542758838588);
	printf("%ld\n",1542758839591 -1542758839591);
	printf("%ld\n",1542758840589 -1542758840589);
	printf("%ld\n",1542758841589 -1542758841589);
	printf("%ld\n",1542758842590 -1542758842590);
	printf("%ld\n",1542758843591 -1542758843591);
	printf("%ld\n",1542758844589 -1542758844589);
	printf("%ld\n",1542758845589 -1542758845589);
	printf("%ld\n",1542758846590 -1542758846590);
	printf("%ld\n",1542758847591 -1542758847591);
	printf("%ld\n",1542758848588 -1542758848588);
	printf("%ld\n",1542758849588 -1542758849588);
	printf("%ld\n",1542758850589 -1542758850589);
	printf("%ld\n",1542758851588 -1542758851589);
	printf("%ld\n",1542758852588 -1542758852588);
	printf("%ld\n",1542758853587 -1542758853587);
	printf("%ld\n",1542758854590 -1542758854590);
	printf("%ld\n",1542758855591 -1542758855590);
	printf("%ld\n",1542758856587 -1542758856587);
	printf("%ld\n",1542758857591 -1542758857591);
	printf("%ld\n",1542758858588 -1542758858588);
	printf("%ld\n",1542758859591 -1542758859591);

}

int main(int ac, char **av) {
	cal();
	srand(time(NULL));
	char path[] = "/tmp/log.out";
	logger_init(path);
	Univers univers;
	srand(time(NULL));

	if (demoGui(ac, av, univers)
		|| uiTest(ac, av, univers))
		return (0);
	std::string buffer;
	for (;;) {
		std::cout << "$> ";
		std::getline(std::cin, buffer);
		if (buffer == "autos") {
			univers.create_server();
			univers.getClientTCP_().connect("localhost", "4242");
			sleep(1);
			univers.getClientTCP_().change_state_ready();
			univers.setMapSize(35);
			univers.load_external_library(std::string("Game pro"),
										  std::string(PATH_LIBRARY_SFML));

		}
		if (buffer == "autoc") {
			univers.getClientTCP_().connect("localhost", "4242");
			sleep(1);
			univers.getClientTCP_().change_state_ready();
			univers.setMapSize(35);
			univers.load_external_library(std::string("Game pro"),
										  std::string(PATH_LIBRARY_SFML));
			univers.loop();
		}
		if (buffer == "autocs") {
			std::cout << "connect > ";
			std::getline(std::cin, buffer);
			univers.getClientTCP_().connect(buffer.c_str(), "4242");
			sleep(1);
			univers.getClientTCP_().change_state_ready();
			univers.setMapSize(35);
			univers.load_external_library(std::string("Game pro"),
										  std::string(PATH_LIBRARY_SFML));
			univers.loop();
		}
		if (buffer == "loop") {
			ClientTCP::StartInfo startInfo;
			univers.getClientTCP_()
					.write_socket(
							ClientTCP::add_prefix(START_GAME, &startInfo));
			univers.loop();
		}
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
			univers.setMapSize(35);
			univers.load_external_library(std::string("Game pro"),
										  std::string(PATH_LIBRARY_SFML));

			ClientTCP::StartInfo startInfo;
			univers.getClientTCP_()
					.write_socket(
							ClientTCP::add_prefix(START_GAME, &startInfo));
			univers.loop();
		}
		if (buffer == "game1") {
			univers.setMapSize(35);
			univers.load_external_library(std::string("Game pro"),
										  std::string(PATH_LIBRARY_SFML));
			univers.loop();
		}
		if (buffer == "ui") {
			univers.create_ui();
			univers.getCore_().aState();
			return (0);
		}
	}
}
