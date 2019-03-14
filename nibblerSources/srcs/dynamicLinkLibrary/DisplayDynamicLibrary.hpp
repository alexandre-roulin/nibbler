#pragma once

#include "ADynamicLinkLibrary.hpp"
#include <IDisplay.hpp>

enum eDisplay {
	kDisplaySfmlLibrary = 0,
	kDisplaySdlLibrary,
	kDisplayGlfwLibrary
};

class DisplayDynamicLibrary : public ADynamicLinkLibrary< IDisplay, eDisplay, IDisplay *(*)(int, int, char const *, eDirection) > {
public:
	DisplayDynamicLibrary();
	virtual ~DisplayDynamicLibrary() = default;
	DisplayDynamicLibrary &operator=(const DisplayDynamicLibrary &) = delete;
	DisplayDynamicLibrary(const DisplayDynamicLibrary &) = delete;
};
