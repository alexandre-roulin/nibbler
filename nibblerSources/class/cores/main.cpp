#include <utility>


#include <iostream>
#include <KINU/Entity.hpp>

#include <KINU/World.hpp>


#include <random>
#include <fstream>
#include <gui/Gui.hpp>
#include <boost/program_options.hpp>
#include <logger.h>
#include <ia/KStar.hpp>

void nibbler(Univers &univers) {


	boost::filesystem::path pathSound(NIBBLER_ROOT_PROJECT_PATH);
	pathSound = pathSound / "ressources" / "sound";
	
	univers.addNoise((pathSound / "appear-online.ogg").generic_string());
	univers.addNoise((pathSound / "yes-2.wav").generic_string());
	univers.addNoise((pathSound / "click.wav").generic_string());
	univers.addNoise((pathSound / "slime10.wav").generic_string());
	univers.addNoise((pathSound / "hit17.ogg").generic_string());
	univers.playMusic((pathSound / "zelda.ogg").generic_string());

	std::unique_ptr<Gui> coreSharedPtr;

	while (!univers.isExit()) {
		univers.createCore();
		univers.getCore_()->aState();
		if (univers.isOpenGame_()) {
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
			{1, 0, 0, 0, 0, 1, 1, 0, 0, 1},
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

void printints(std::array<int, 6> const &ints) {
	for (const auto &item : ints) {
		std::cout << item << " ";
	}
	std::cout << std::endl;
}
void trya() {

	std::array<int, 6> ints = {0, 1, 2, 3, 4, 5};
	void *pVoid = static_cast<void *>(ints.data());
	std::array<int, 6> intsCopy;
	std::cout << "Print ints initialized" << std::endl;
	printints(ints);
	std::cout << "Print ints copy empty" << std::endl;
	printints(intsCopy);
	std::memcpy(&intsCopy, pVoid, sizeof(ints));
	std::cout << "Print ints copy full" << std::endl;
	printints(intsCopy);

}

void tryi() {
	std::array<int, 6> connectionsId({ -1, -1, -1, -1, -1, -1 });


	printints(connectionsId);

	std::cout << std::distance(connectionsId.begin(), std::find(connectionsId.begin(), connectionsId.end(), -1)) << std::endl;
	connectionsId[0] = 2;
	std::cout << std::distance(connectionsId.begin(), std::find(connectionsId.begin(), connectionsId.end(), -1)) << std::endl;
}

int main(int argc, char **argv) {
//	tryi();
//	testKstar();
//	return 1;

	if (!NIBBLER_ROOT_PROJECT_PATH) {
		std::cerr << "NIBBLER_ROOT_PROJECT_PATH is not defined" << std::endl;
		return (0);
	}
	char hostset[64] = "Kryssou";
	sethostname(hostset, 64);
	char hostname[64];
	gethostname(hostname, 64);
	std::cout << hostname << std::endl;
	srand(time(NULL));
	char path[] = "/tmp/log.out";
	if (argc > 1) {
		logger_init(argv[1]);
	} else {
		logger_init(path);
	}
	try {
		Univers univers;

		boost::program_options::options_description desc("Options");
		desc.add_options()
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
			if (vm.count("sound"))
				univers.load_extern_lib_sound(Univers::eSound::kSoundSfmlLibrary);
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
