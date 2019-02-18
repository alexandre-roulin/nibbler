#include <utility>
#include <iostream>
#include <random>
#include <fstream>
#include <gui/Gui.hpp>
#include <boost/program_options.hpp>
#include <logger.h>
#include <ia/KStar.hpp>


void nibbler(Univers &univers) {
	boost::filesystem::path pathSound(NIBBLER_ROOT_PROJECT_PATH);
	pathSound = pathSound / "ressources" / "sound";

	univers.getSoundManager().addNoise((pathSound / "appear-online.ogg").generic_string());
	univers.getSoundManager().addNoise((pathSound / "yes-2.wav").generic_string());
	univers.getSoundManager().addNoise((pathSound / "click.wav").generic_string());
	univers.getSoundManager().addNoise((pathSound / "slime10.wav").generic_string());
	univers.getSoundManager().addNoise((pathSound / "hit17.ogg").generic_string());
	univers.getSoundManager().playMusic((pathSound / "zelda.ogg").generic_string());

	while (!univers.isExit()) {
		univers.createGui();
		univers.getGui_()->aState();
		univers.deleteGui();

		if (univers.isOpenGame_()) {
			univers.startNewGame();
		}

	}
}


int main(int argc, char **argv) {
	if (!NIBBLER_ROOT_PROJECT_PATH) {
		std::cerr << "NIBBLER_ROOT_PROJECT_PATH is not defined" << std::endl;
		return (0);
	}

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
				univers.getSoundManager().loadExternalSoundLibrary(eSound::kSoundSfmlLibrary);
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
