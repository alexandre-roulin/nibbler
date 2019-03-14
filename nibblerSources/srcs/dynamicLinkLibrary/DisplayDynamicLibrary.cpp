#include "DisplayDynamicLibrary.hpp"
#include <boost/filesystem.hpp>
#include <nibbler.hpp>

DisplayDynamicLibrary::DisplayDynamicLibrary() :
	ADynamicLinkLibrary< IDisplay, eDisplay, IDisplay *(*)(int, int, char const *, eDirection) >(kDisplaySfmlLibrary) {
	boost::filesystem::path pathDynamicLibraries(NIBBLER_BUILD_PROJECT_PATH);

			ADynamicLinkLibrary<IDisplay, eDisplay, IDisplay *(*)(int, int, char const *, eDirection)>::libraryInfo.reserve(3);
	ADynamicLinkLibrary<IDisplay, eDisplay, IDisplay *(*)(int, int, char const *, eDirection)>::libraryInfo.emplace_back(
			"Nibbler - SFML", (pathDynamicLibraries / "dynamicLibraries/libdisplay_sfml.so").generic_path().c_str(), eDisplay::kDisplaySfmlLibrary);
	ADynamicLinkLibrary<IDisplay, eDisplay, IDisplay *(*)(int, int, char const *, eDirection)>::libraryInfo.emplace_back(
			"Nibbler - SDL", (pathDynamicLibraries / "dynamicLibraries/libdisplay_sdl.so").generic_path().c_str(), eDisplay::kDisplaySdlLibrary);
	ADynamicLinkLibrary<IDisplay, eDisplay, IDisplay *(*)(int, int, char const *, eDirection)>::libraryInfo.emplace_back(
			"Nibbler - GLFW", (pathDynamicLibraries / "dynamicLibraries/libdisplay_glfw.so").generic_path().c_str(), eDisplay::kDisplayGlfwLibrary);
	setNextKInstance(kDisplaySfmlLibrary);
}