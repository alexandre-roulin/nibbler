#include <utility>
#include <iostream>
#include <random>
#include <fstream>
#include <gui/Gui.hpp>
#include <boost/program_options.hpp>

void nibbler(Univers &univers) {
	while (!univers.isExit()) {
		if (univers.getGui_() == nullptr)
			univers.createGui();
		univers.getGui_()->mainLoop();

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
	try {
		Univers univers;

		boost::program_options::options_description desc("Options");
		desc.add_options()
				("help,h", "Print help messages");

		boost::program_options::variables_map vm;
		try {
			boost::program_options::store(
					boost::program_options::parse_command_line(
							argc, argv, desc), vm);

			if (vm.count("help")) {
				std::cout << "Basic Command Line Parameter App" << std::endl
						  << desc << std::endl;
				return (0);
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
	return (0);
}
