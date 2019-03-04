#pragma once

#include <IDisplay.hpp>
#include <array>
#include "nibbler.hpp"
class Univers;

class ExternalLibraryDisplayManager {

public:
	ExternalLibraryDisplayManager(Univers &univers);
	virtual ~ExternalLibraryDisplayManager();
	ExternalLibraryDisplayManager &operator=(const ExternalLibraryDisplayManager &) = delete;
	ExternalLibraryDisplayManager(const ExternalLibraryDisplayManager &) = delete;

	void switchNextLibrary();
	void loadExternalDisplayLibrary(eDisplay display);
	void constructExternalLibrary(int width, int height);
	void unloadExternalDisplayLibrary();
	void dlError();
	bool hasLibraryLoaded() const;
	bool hasConstructorLoaded() const;
	IDisplay *getDisplay() const;
	eDisplay getKDisplay() const;
	void setKDisplay(eDisplay k);

	static constexpr LibraryInfo libraryInfo[sizeof(eDisplayArray) / sizeof(eDisplay)] = {
			{"Nibbler - SFML", "dynamicLibraries/libdisplay_sfml.so", eDisplay::kDisplaySfmlLibrary}, //[kDisplaySfmlLibrary] = 0
			{"Nibbler - SDL", "dynamicLibraries/libdisplay_sdl.so", eDisplay::kDisplaySdlLibrary}, //[kDisplaySdlLibrary] = 1
			{"Nibbler - GLFW", "dynamicLibraries/libdisplay_glfw.so", eDisplay::kDisplayGlfwLibrary} //[kDisplayGlfwLibrary] = 2
	};

private:

	void *dlHandleDisplay;
	IDisplay *(*newDisplay)(int, int, char const *);
	IDisplay *display;
	void (*deleteDisplay)(IDisplay *);
	eDisplay kDisplay;
	Univers &univers_;
};