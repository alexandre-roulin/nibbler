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
#include <ia/KStar.hpp>

std::string const Snake::basicName[SNAKE_MAX_NAME] = {"Jack O'Lantern", "Eden",
												 "Jacky", "Emerald",
												 "Broutille", "Veggie-vie",
												 "jinou42", "Dautta c bo"};


std::ostream &operator<<(std::ostream &os, const Snake &snake) {
	os << " id: " << snake.id << " isAlive: " << snake.isAlive;
	return os;
}

/*
	univers.getSound().addNoise(
			std::string("./ressource/sound/appear-online.ogg"));
	univers.getSound().addNoise(std::string("./ressource/sound/yes-2.wav"));
	univers.getSound().addNoise(std::string("./ressource/sound/click.wav"));
	univers.getSound().addNoise(
			std::string("./ressource/sound/slime10.wav"));
	univers.getSound().addNoise(std::string("./ressource/sound/hit17.ogg"));
	univers.getSound().setMusic("./ressource/sound/zelda.ogg");
	univers.getSound().playMusic();
*/

void nibbler(Univers &univers) {
	while (1) {

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


void testKstar() {
	int size = 10;

	KStar kStar;
	KStar::Vec2 source(1, 1);
	KStar::Vec2 target(9, 9);
	kStar.setWorldSize(size);
	kStar.setHeuristic(KStar::Heuristic::euclidean);
	kStar.setDiagonalMovement(false);

	int collision[10][10] = {
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
			{1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
			{1, 3, 0, 0, 0, 1, 0, 0, 0, 1},
			{1, 3, 2, 1, 1, 1, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	};

	for (int y = 0; y < 10; ++y) {
		for (int x = 0; x < 10; ++x) {
			if (collision[y][x] == 1)
				kStar.addCollision(KStar::Vec2(x, y));
			if (collision[y][x] == 2)
				source = KStar::Vec2(x, y);
			if (collision[y][x] == 3)
				target = KStar::Vec2(x, y);
		}

	}

	for (int iy = 0; iy < size; ++iy) {
		for (int ix = 0; ix < size; ++ix) {
			if (iy == source.y && ix == source.x)
				std::cout << std::setw(4) << "_^_";
			else if (iy == target.y && ix == target.x)
				std::cout << std::setw(4) << "_O_";
			else if (kStar.isCollision(KStar::Vec2(ix, iy)))
				std::cout << std::setw(4) << "_X_";
			else
				std::cout << std::setw(4) << "_._";
		}
		std::cout << std::endl;
	}
	KStar::Path path = kStar.searchPath(source, target);
	std::cout << std::endl;
	std::cout << path.size() << std::endl;
	std::cout << std::endl;

	for (int iy = 0; iy < size; ++iy) {
		for (int ix = 0; ix < size; ++ix) {
			if (iy == source.y && ix == source.x)
				std::cout << std::setw(4) << "_^_";
			else if (iy == target.y && ix == target.x)
				std::cout << std::setw(4) << "_O_";
			else if (kStar.isCollision(KStar::Vec2(ix, iy)))
				std::cout << std::setw(4) << "_X_";
			else if (std::find_if(path.begin(), path.end(), [ix, iy](KStar::Vec2 vec){ return vec.x == ix && vec.y == iy; }) != path.end())
				std::cout << std::setw(4) << "_#_";
			else
				std::cout << std::setw(4) << "_._";
		}
		std::cout << std::endl;
	}
}

//.O
//
//

int main(int argc, char **argv) {
//	testKstar();
//	return 1;

	if (!NIBBLER_ROOT_PROJECT_PATH) {
		std::cerr << "NIBBLER_ROOT_PROJECT_PATH is not defined" << std::endl;
		return (0);
	}

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
		nibbler(univers);
	}
	catch (const std::exception &e) {
		std::cerr << "Unhandled Exception reached the top of main: "
				  << e.what() << ", application will now exit" << std::endl;
	}
	log_success("main.return()");
	return (0);
}
