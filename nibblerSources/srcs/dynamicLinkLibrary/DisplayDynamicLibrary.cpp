#include "DisplayDynamicLibrary.hpp"

DisplayDynamicLibrary::DisplayDynamicLibrary() :
		ADynamicLinkLibrary< IDisplay, eDisplay, IDisplay *(*)(int, int, char const *, eDirection) >(kDisplaySfmlLibrary) {

	ADynamicLinkLibrary<IDisplay, eDisplay, IDisplay *(*)(int, int, char const *, eDirection)>::libraryInfo.reserve(3);
	ADynamicLinkLibrary<IDisplay, eDisplay, IDisplay *(*)(int, int, char const *, eDirection)>::libraryInfo.emplace_back(
			"Nibbler - SFML", "dynamicLibraries/libdisplay_sfml.so", eDisplay::kDisplaySfmlLibrary);
	ADynamicLinkLibrary<IDisplay, eDisplay, IDisplay *(*)(int, int, char const *, eDirection)>::libraryInfo.emplace_back(
			"Nibbler - SDL", "dynamicLibraries/libdisplay_sdl.so", eDisplay::kDisplaySdlLibrary);
	ADynamicLinkLibrary<IDisplay, eDisplay, IDisplay *(*)(int, int, char const *, eDirection)>::libraryInfo.emplace_back(
			"Nibbler - GLFW", "dynamicLibraries/libdisplay_glfw.so", eDisplay::kDisplayGlfwLibrary);
	setNextKInstance(kDisplayGlfwLibrary);
}