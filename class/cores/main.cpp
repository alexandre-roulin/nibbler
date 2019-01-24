#include <utility>


#include <iostream>
#include <KINU/Entity.hpp>

#include <KINU/World.hpp>


#include <random>
#include <fstream>
#include <network/ServerTCP.hpp>
#include <network/ClientTCP.hpp>
#include <gui/Core.hpp>
#include <boost/program_options.hpp>
#include <logger.h>

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

void nibbler(Univers &univers) {
	std::string buffer;
	if (univers.testFlag(Univers::SOUND)) {
		univers.load_external_sound_library(std::string("Game pro"),
											std::string(
													PATH_SOUND_LIBRARY_SFML));

		univers.getSound().addNoise(
				std::string("./ressource/sound/appear-online.ogg"));
		univers.getSound().addNoise(std::string("./ressource/sound/yes-2.wav"));
		univers.getSound().addNoise(std::string("./ressource/sound/click.wav"));
		univers.getSound().addNoise(
				std::string("./ressource/sound/slime10.wav"));
		univers.getSound().addNoise(std::string("./ressource/sound/hit17.ogg"));
		/*
		univers.getSound().setMusic("./ressource/sound/zelda.ogg");
		univers.getSound().playMusic();
		*/
	}

	for (;;) {
		std::cout << "$> ";
		std::getline(std::cin, buffer);
		if (buffer == "closea") {
			univers.close_acceptor();
		}
		if (buffer == "border") {
			univers.setBorderless(true);
		}
		if (buffer == "ia") {
			univers.create_ia();
		}
		if (buffer == "dlia") {
			univers.delete_ia();
		}
		if (buffer == "dls") {
			univers.delete_server();
		}
		if (buffer == "sfml") {
			univers.load_extern_lib_display(Univers::kExternSfmlLibrary);
		}
		if (buffer == "autos") {
			univers.create_server();
			univers.getGameNetwork()->connect("localhost", "4242");
			sleep(1);
			univers.getGameNetwork()->change_state_ready();
			univers.load_extern_lib_display(Univers::kExternSfmlLibrary);

		}
		if (buffer == "autoc") {
			univers.getGameNetwork()->connect("localhost", "4242");
			sleep(1);
			univers.getGameNetwork()->change_state_ready();
			univers.load_extern_lib_display(Univers::kExternSfmlLibrary);
			univers.new_game();
		}
		if (buffer == "autocs") {
			std::cout << "connect > ";
			std::getline(std::cin, buffer);
			univers.getGameNetwork()->connect(buffer.c_str(), "4242");
			sleep(1);
			univers.getGameNetwork()->change_state_ready();
			univers.load_extern_lib_display(Univers::kExternSfmlLibrary);
			univers.loop();
		}
		if (buffer == "loop") {
			univers.new_game();
		}
		if (buffer == "server") {
			univers.create_server();
		}
		if (buffer == "connect") {
			std::string dns, port;
			std::cout << "dns > ";
			std::getline(std::cin, dns);
			std::cout << "port > ";
			std::getline(std::cin, port);
			univers.getGameNetwork()->connect(dns, port);
		}
		if (buffer == "ready") {
			univers.getGameNetwork()->change_state_ready();
		}

		if (buffer == "ui") {
			univers.create_ui();
			univers.getCore_().aState();

			bool start = univers.getCore_().getStartGane();
			Core *core = univers.releaseCore_();
			if (core)
				delete core;
			if (start) {
				univers.load_extern_lib_display(Univers::kExternSfmlLibrary);
				univers.new_game();
			}
		}
	}
}

int main(int argc, char **argv) {

	char hostname[64];
	gethostname(hostname, 64);
	std::cout << hostname << std::endl;
	srand(time(NULL));
	char path[] = "/tmp/log.out";
	logger_init(path);
	try {
		Univers univers;

		boost::program_options::options_description desc("Options");
		desc.add_options() // <- Retourne une reference ! OP
				("help", "Print help messages")
				("sound", "enable the sound");

		boost::program_options::variables_map vm;
		try {
			boost::program_options::store(
					boost::program_options::parse_command_line(argc, argv,
															   desc), vm);

			if (vm.count("help")) {
				std::cout << "Basic Command Line Parameter App" << std::endl
						  << desc << std::endl;
				return (0);
			}
			if (vm.count("sound")) {
				univers.setFlag(Univers::SOUND);
			}
			boost::program_options::notify(vm);
		}
		catch (const boost::program_options::error &e) {
			std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
			std::cerr << desc << std::endl;
			return (0);
		}
		if (demoGui(argc, argv, univers))
			return (0);
		nibbler(univers);
	}
	catch (const std::exception &e) {
		std::cerr << "Unhandled Exception reached the top of main: "
				  << e.what() << ", application will now exit" << std::endl;
	}

	return (0);
}
