#pragma once

#include <IDisplay.hpp>
#include <array>
#include "nibbler.hpp"

class ExternalLibraryManager {
	struct LibraryInfo {
		char const title[80];
		char const path[PATH_MAX];
	};
	ExternalLibraryManager();
	void switchNextLibrary();
	void loadExternalDisplayLibrary(eDisplay display);
	void constructExternalLibrary(int, int);
	void unloadExternalDisplayLibrary();
	void dlError(char const *from);
	bool hasLibraryLoaded() const;
private:

	static constexpr LibraryInfo libraryInfo[sizeof(eDisplayArray) / sizeof(eDisplay)] = {
			[kDisplaySfmlLibrary] = {"Nibbler - SFML", "dynamicLibraries/libdisplay_sfml.so"},
			[kDisplaySdlLibrary] = {"Nibbler - SDL", "dynamicLibraries/libdisplay_sdl.so"},
			[kDisplayGlfwLibrary] = {"Nibbler - GLFW", "dynamicLibraries/libdisplay_glfw.so"}
	};

	void *dlHandleDisplay;
	IDisplay *(*newDisplay)(int, int, char const *);
	IDisplay *display;
	void (*deleteDisplay)(IDisplay *);
	eDisplay kDisplay;
};