#include <utility>
#include <iostream>
#include <random>
#include <fstream>
#include <gui/Gui.hpp>
#include <boost/program_options.hpp>
#include <logger.h>
#include <ia/KStar.hpp>
#include "cores/Test.hpp"

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

void option_dependency(boost::program_options::variables_map const &vm,
					   std::string const &for_what, std::string const &required_option) {
	if (vm.count(for_what) && !vm[for_what].defaulted())
		if (vm.count(required_option) == 0 || vm[required_option].defaulted())
			throw std::logic_error(std::string("Option '") + for_what
								   + "' requires option '" + required_option + "'.");
}

template<typename ... Args>
void option_dependency(boost::program_options::variables_map const &vm,
					   std::string const &for_what,
					   std::string const &required_option,
					   Args ... args) {
	option_dependency(vm, for_what, required_option);
	option_dependency(vm, for_what, args...);
}


int main(int argc, char **argv) {
	if (!NIBBLER_ROOT_PROJECT_PATH) {
		std::cerr << "NIBBLER_ROOT_PROJECT_PATH is not defined" << std::endl;
		return 0;
	}

	srand(time(NULL));
	char path[] = "/tmp/log.out";
	try {
		Univers univers;

		boost::program_options::options_description desc("Options");
		desc.add_options()
				("help,h", "Print help messages")
				("fileInput", boost::program_options::value<std::string>(), "File to store input")
				("id", boost::program_options::value<int>(), "Id of input")
				("pidTestProcess", boost::program_options::value<int>(), "Pid of shell tester process")
				("test,t", "Boolean for test mode")
				("input,i", "Boolean for input mode")
				("logger,l", boost::program_options::value<std::string>(), "Set file for outpout log")
				("sound,s", "enable the sound");

		boost::program_options::variables_map vm;
		try {
			boost::program_options::store(
					boost::program_options::parse_command_line(argc, argv,
															   desc), vm);

			option_dependency(vm, "test", "id", "fileInput", "pidTestProcess");
			option_dependency(vm, "input", "id", "fileInput");

			if (vm.count("help")) {
				std::cout << "Basic Command Line Parameter App" << std::endl
						  << desc << std::endl;
				return (0);
			}
			if (vm.count("sound"))
				univers.getSoundManager().loadExternalSoundLibrary(eSound::kSoundSfmlLibrary);
			if (vm.count("test") && vm.count("id") && vm.count("fileInput") && vm.count("pidTestProcess")) {
				Test::getInstance().setTest(true);
				Test::getInstance().setId(vm["id"].as<int>());
				Test::getInstance().setPidTestProcess(vm["pidTestProcess"].as<int>());
				Test::getInstance().setInputFile(vm["fileInput"].as<std::string>());
			}
			else if (vm.count("input") && vm.count("id") && vm.count("fileInput")) {
				Test::getInstance().setInput(true);
				Test::getInstance().setId(vm["id"].as<int>());
				Test::getInstance().setInputFile(vm["fileInput"].as<std::string>());
			}

			if (vm.count("logger"))
				logger_init(const_cast<char *>(vm["logger"].as<std::string>().c_str()));
			else
				logger_init(path);
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
