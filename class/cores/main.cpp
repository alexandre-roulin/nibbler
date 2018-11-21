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
												 "jinou42", "Dautta c bo"};

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


	printf("%ld\n", 1542654256474 - 1542654256474);
	printf("%ld\n", 1542654257474 - 1542654257474);
	printf("%ld\n", 1542654258474 - 1542654258474);
	printf("%ld\n", 1542654259474 - 1542654259474);
	printf("%ld\n", 1542654260475 - 1542654260475);
	printf("%ld\n", 1542654261474 - 1542654261474);
	printf("%ld\n", 1542654262474 - 1542654262474);
	printf("%ld\n", 1542654263474 - 1542654263474);
	printf("%ld\n", 1542654264474 - 1542654264474);
	printf("%ld\n", 1542654265474 - 1542654265474);
	printf("%ld\n", 1542654266474 - 1542654266474);
	printf("%ld\n", 1542654267474 - 1542654267474);
	printf("%ld\n", 1542654268474 - 1542654268474);
	printf("%ld\n", 1542654269475 - 1542654269475);
	printf("%ld\n", 1542654270474 - 1542654270474);
	printf("%ld\n", 1542654271474 - 1542654271474);
	printf("%ld\n", 1542654272473 - 1542654272473);
	printf("%ld\n", 1542654273474 - 1542654273474);
	printf("%ld\n", 1542654274475 - 1542654274475);
	printf("%ld\n", 1542654275474 - 1542654275474);
	printf("%ld\n", 1542654276474 - 1542654276474);
	printf("%ld\n", 1542654277473 - 1542654277473);
	printf("%ld\n", 1542654278475 - 1542654278475);
	printf("%ld\n", 1542654279473 - 1542654279473);
	printf("%ld\n", 1542654280473 - 1542654280473);
	printf("%ld\n", 1542654281475 - 1542654281475);
	printf("%ld\n", 1542654282475 - 1542654282475);
	printf("%ld\n", 1542654283474 - 1542654283474);
	printf("%ld\n", 1542654284474 - 1542654284474);
	printf("%ld\n", 1542654285474 - 1542654285474);
	printf("%ld\n", 1542654286475 - 1542654286475);
	printf("%ld\n", 1542654287474 - 1542654287474);
	printf("%ld\n", 1542654288473 - 1542654288473);
	printf("%ld\n", 1542654289475 - 1542654289475);
	printf("%ld\n", 1542654290474 - 1542654290474);
	printf("%ld\n", 1542654291473 - 1542654291473);
	printf("%ld\n", 1542654292474 - 1542654292474);
	printf("%ld\n", 1542654293473 - 1542654293473);
	printf("%ld\n", 1542654294474 - 1542654294474);
	printf("%ld\n", 1542654295473 - 1542654295473);
	printf("%ld\n", 1542654296474 - 1542654296474);
	printf("%ld\n", 1542654297472 - 1542654297472);
	printf("%ld\n", 1542654298475 - 1542654298475);
	printf("%ld\n", 1542654299472 - 1542654299472);
	printf("%ld\n", 1542654300475 - 1542654300475);
	printf("%ld\n", 1542654301472 - 1542654301472);

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
	
	univers.load_external_sound_library(std::string("Game pro"),
								  std::string(PATH_SOUND_LIBRARY_SFML));
	
	univers.getSound().addNoise(std::string("./ressource/sound/appear-online.ogg"));
	univers.getSound().addNoise(std::string("./ressource/sound/yes-2.wav"));
	univers.getSound().addNoise(std::string("./ressource/sound/click.wav"));
	univers.getSound().addNoise(std::string("./ressource/sound/slime10.wav"));
	univers.getSound().addNoise(std::string("./ressource/sound/hit17.ogg"));
	/*
	univers.getSound().setMusic("./ressource/sound/zelda.ogg");
	univers.getSound().playMusic();
	*/
	
	for (;;) {
		std::cout << "$> ";
		std::getline(std::cin, buffer);
		if (buffer == "autos") {
			univers.create_server();
			univers.getClientTCP_().connect("localhost", "4242");
			sleep(1);
			univers.getClientTCP_().change_state_ready();
			univers.setMapSize(35);
			univers.load_external_display_library(std::string("Game pro"),
										  std::string(PATH_DISPLAY_LIBRARY_SFML));

		}
		if (buffer == "autoc") {
			univers.getClientTCP_().connect("localhost", "4242");
			sleep(1);
			univers.getClientTCP_().change_state_ready();
			univers.setMapSize(35);
			univers.load_external_display_library(std::string("Game pro"),
										  std::string(PATH_DISPLAY_LIBRARY_SFML));
			univers.loop();
		}
		if (buffer == "autocs") {
			std::cout << "connect > ";
			std::getline(std::cin, buffer);
			univers.getClientTCP_().connect(buffer.c_str(), "4242");
			sleep(1);
			univers.getClientTCP_().change_state_ready();
			univers.setMapSize(35);
			univers.load_external_display_library(std::string("Game pro"),
										  std::string(PATH_DISPLAY_LIBRARY_SFML));
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
			univers.load_external_display_library(std::string("Game pro"),
										  std::string(PATH_DISPLAY_LIBRARY_SFML));

			ClientTCP::StartInfo startInfo;
			univers.getClientTCP_()
					.write_socket(
							ClientTCP::add_prefix(START_GAME, &startInfo));
			univers.loop();
		}
		if (buffer == "game1") {
			univers.setMapSize(35);
			univers.load_external_display_library(std::string("Game pro"),
										  std::string(PATH_DISPLAY_LIBRARY_SFML));
			univers.loop();
		}
		if (buffer == "ui") {
			univers.create_ui();
			univers.getCore_().aState();
			Core *core = univers.releaseCore_();
			if (core)
				delete core;
			
			if (univers.getClientTCP_().isOpenGame()) {
				
				//univers.getClientTCP_().change_state_ready();
				//sleep(1);
				
				univers.load_external_display_library(std::string("Game pro"),
											  std::string(PATH_DISPLAY_LIBRARY_SFML));

			  ClientTCP::StartInfo startInfo;
				if (univers.isServer()) {
					univers.getClientTCP_()
							.write_socket(
									ClientTCP::add_prefix(START_GAME, &startInfo));	
				} else {
					std::cout << "I launch " << std::endl;
				}
				univers.loop();
			}
			return (0);
		}
	}
}
