#pragma once

#include <IDisplay.hpp>
#include <array>
#include "nibbler.hpp"

class ExternalLibraryDisplayManager {

public:
	ExternalLibraryDisplayManager();
	virtual ~ExternalLibraryDisplayManager();
	ExternalLibraryDisplayManager &operator=(const ExternalLibraryDisplayManager &) = delete;
	ExternalLibraryDisplayManager(const ExternalLibraryDisplayManager &) = delete;

	void switchNextLibrary();
	void loadExternalDisplayLibrary(eDisplay display);
	void constructExternalLibrary(int width, int height);
	void unloadExternalDisplayLibrary();
	void dlError(char const *from);
	bool hasLibraryLoaded() const;
	IDisplay *getDisplay() const;
	eDisplay getKDisplay() const;

private:

	static constexpr LibraryInfo libraryInfo[sizeof(eDisplayArray) / sizeof(eDisplay)] = {
			{"Nibbler - SFML", "dynamicLibraries/libdisplay_sfml.so"}, //[kDisplaySfmlLibrary] = 0
			{"Nibbler - SDL", "dynamicLibraries/libdisplay_sdl.so"}, //[kDisplaySdlLibrary] = 1
			{"Nibbler - GLFW", "dynamicLibraries/libdisplay_glfw.so"} //[kDisplayGlfwLibrary] = 2
	};

	void *dlHandleDisplay;
	IDisplay *(*newDisplay)(int, int, char const *);
	IDisplay *display;
	void (*deleteDisplay)(IDisplay *);
	eDisplay kDisplay;
};