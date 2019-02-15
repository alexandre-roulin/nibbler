#include <dlfcn.h>
#include "ExternalLibraryDisplayManager.hpp"
#include <boost/filesystem/path.hpp>

ExternalLibraryDisplayManager::ExternalLibraryDisplayManager():
	dlHandleDisplay(nullptr),
	newDisplay(nullptr),
	display(nullptr),
	deleteDisplay(nullptr),
	kDisplay(kDisplaySfmlLibrary) {
}

void ExternalLibraryDisplayManager::loadExternalDisplayLibrary(eDisplay display) {
	if (!(dlHandleDisplay = dlopen(libraryInfo[display].path, RTLD_LAZY | RTLD_LOCAL))) {
		dlError("dlopen");
		return;
	}
	if (!(newDisplay = reinterpret_cast<IDisplay *(*)(
			int, int, const char *)>(dlsym(dlHandleDisplay, "newDisplay")))) {
		dlError("dlsym - new");
		return;
	}
	if (!(deleteDisplay = reinterpret_cast<void (*)(IDisplay *
	)>(dlsym(dlHandleDisplay, "deleteDisplay")))) {
		dlError("dlsym - delete");
		return;
	}
}

void ExternalLibraryDisplayManager::unloadExternalDisplayLibrary() {
	if (display != nullptr && dlHandleDisplay != nullptr) {
		if (deleteDisplay) {
			deleteDisplay(display);
			deleteDisplay = nullptr;
			newDisplay = nullptr;
			display = nullptr;
		}
		dlclose(dlHandleDisplay);
		dlHandleDisplay = nullptr;
	}
}

void ExternalLibraryDisplayManager::switchNextLibrary() {
	unloadExternalDisplayLibrary();
	kDisplay = (kDisplay == kDisplayGlfwLibrary) ? kDisplaySfmlLibrary : static_cast<eDisplay>(kDisplay + 1);
	loadExternalDisplayLibrary(kDisplay);
}


bool ExternalLibraryDisplayManager::hasLibraryLoaded() const {
	return display != nullptr;
}


void ExternalLibraryDisplayManager::dlError(char const *from) {
	std::cerr << "Error " << from << " : " << dlerror() << std::endl;
	unloadExternalDisplayLibrary();
}

void ExternalLibraryDisplayManager::constructExternalLibrary(int width, int height) {
	assert(newDisplay != nullptr);
	display = newDisplay(width, height, libraryInfo[kDisplay].title);
}

IDisplay *ExternalLibraryDisplayManager::getDisplay() const {
	return display;
}

eDisplay ExternalLibraryDisplayManager::getKDisplay() const {
	return kDisplay;
}

ExternalLibraryDisplayManager::~ExternalLibraryDisplayManager() {
	unloadExternalDisplayLibrary();
}
