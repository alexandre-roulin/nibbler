#include <dlfcn.h>
#include "ExternalLibraryDisplayManager.hpp"
#include <boost/filesystem/path.hpp>
#include <gui/Gui.hpp>

ExternalLibraryDisplayManager::ExternalLibraryDisplayManager(Univers & univers):
	dlHandleDisplay(nullptr),
	newDisplay(nullptr),
	display(nullptr),
	deleteDisplay(nullptr),
	kDisplay(kDisplaySfmlLibrary),
	univers_(univers) {
}

void ExternalLibraryDisplayManager::loadExternalDisplayLibrary(eDisplay display) {
	if (!(dlHandleDisplay = dlopen(libraryInfo[display].path, RTLD_LAZY | RTLD_LOCAL))) {
		dlError();
		return;
	}
	if (!(newDisplay = reinterpret_cast<IDisplay *(*)(
			int, int, const char *)>(dlsym(dlHandleDisplay, "newDisplay")))) {
		dlError();
		return;
	}
	if (!(deleteDisplay = reinterpret_cast<void (*)(IDisplay *
	)>(dlsym(dlHandleDisplay, "deleteDisplay")))) {
		dlError();
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


void ExternalLibraryDisplayManager::dlError() {
	std::unique_ptr<Gui> &gui = univers_.getGui_();
	if (gui)
		gui->addMessageChat(eColorLog::kRed, dlerror());
	unloadExternalDisplayLibrary();
}

void ExternalLibraryDisplayManager::constructExternalLibrary(int width, int height) {
	assert(newDisplay != nullptr);
	display = newDisplay(width, height, libraryInfo[kDisplay].title);
}

IDisplay *ExternalLibraryDisplayManager::getDisplay() const {
	return display;
}

void ExternalLibraryDisplayManager::setKDisplay(eDisplay k) {
	kDisplay = k;
}
eDisplay ExternalLibraryDisplayManager::getKDisplay() const {
	return kDisplay;
}

ExternalLibraryDisplayManager::~ExternalLibraryDisplayManager() {
	unloadExternalDisplayLibrary();
}

bool ExternalLibraryDisplayManager::hasConstructorLoaded() const {
	return	newDisplay != nullptr &&
			dlHandleDisplay != nullptr &&
			deleteDisplay != nullptr;
}
