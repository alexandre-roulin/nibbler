#include <dlfcn.h>
#include "ExternalLibraryManager.hpp"
#include <boost/filesystem/path.hpp>

ExternalLibraryManager::ExternalLibraryManager():
	dlHandleDisplay(nullptr),
	newDisplay(nullptr),
	display(nullptr),
	deleteDisplay(nullptr),
	kDisplay(kDisplaySfmlLibrary) {
}

void ExternalLibraryManager::loadExternalDisplayLibrary(eDisplay display) {
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

void ExternalLibraryManager::unloadExternalDisplayLibrary() {
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

void ExternalLibraryManager::switchNextLibrary() {
	unloadExternalDisplayLibrary();
	kDisplay = (kDisplay == kDisplayGlfwLibrary) ? kDisplaySfmlLibrary : static_cast<eDisplay>(kDisplay + 1);
	loadExternalDisplayLibrary(kDisplay);
}


bool ExternalLibraryManager::hasLibraryLoaded() const {
	return display != nullptr;
}


void ExternalLibraryManager::dlError(char const *from) {
	std::cerr << "Error " << from << " : " << dlerror() << std::endl;
	dlclose(dlHandleDisplay);
}

void ExternalLibraryManager::constructExternalLibrary(int width, int height) {
	assert(newDisplay != nullptr);
	display = newDisplay(width, height, libraryInfo[kDisplay].title);
}
