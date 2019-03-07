//#include <dlfcn.h>
//#include "DisplayDynamicLibrary.hpp"
//#include <boost/filesystem/path.hpp>
//#include <gui/Gui.hpp>
//
//DisplayDynamicLibrary::DisplayDynamicLibrary(Univers & univers):
//	dlHandleDisplay(nullptr),
//	newDisplay(nullptr),
//	display(nullptr),
//	deleteDisplay(nullptr),
//	kDisplay(kDisplaySfmlLibrary),
//	univers_(univers) {
//}
//
//void DisplayDynamicLibrary::loadExternalDisplayLibrary(eDisplay display) {
//	if (!(dlHandleDisplay = dlopen(libraryInfo[display].path, RTLD_LAZY | RTLD_LOCAL))) {
//		dlError();
//		return;
//	}
//	if (!(newDisplay = reinterpret_cast<
//			IDisplay *(*)(int, int, const char *)
//			>(dlsym(dlHandleDisplay, "newDisplay")))) {
//		dlError();
//		return;
//	}
//	if (!(deleteDisplay = reinterpret_cast<
//			void (*)(IDisplay *)
//			>(dlsym(dlHandleDisplay, "deleteInstance")))) {
//		dlError();
//		return;
//	}
//}
//
//void DisplayDynamicLibrary::unloadExternalDisplayLibrary() {
//	if (display != nullptr && dlHandleDisplay != nullptr) {
//		if (deleteDisplay) {
//			deleteDisplay(display);
//			deleteDisplay = nullptr;
//			newDisplay = nullptr;
//			display = nullptr;
//		}
//		dlclose(dlHandleDisplay);
//		dlHandleDisplay = nullptr;
//	}
//}
//
//void DisplayDynamicLibrary::switchNextLibrary() {
//	unloadExternalDisplayLibrary();
//	kDisplay = (kDisplay == kDisplayGlfwLibrary) ? kDisplaySfmlLibrary : static_cast<eDisplay>(kDisplay + 1);
//	loadExternalDisplayLibrary(kDisplay);
//}
//
//
//bool DisplayDynamicLibrary::hasLibraryLoaded() const {
//	return display != nullptr;
//}
//
//
//void DisplayDynamicLibrary::dlError() {
//	std::unique_ptr<Gui> &gui = univers_.getGui_();
//	if (gui)
//		gui->addMessageChat(eColorLog::kRed, dlerror());
//	unloadExternalDisplayLibrary();
//}
//
//void DisplayDynamicLibrary::constructExternalLibrary(int width, int height) {
//	assert(newDisplay != nullptr);
//	display = newDisplay(width, height, libraryInfo[kDisplay].title);
//}
//
//IDisplay *DisplayDynamicLibrary::getDisplay() const {
//	return display;
//}
//
//void DisplayDynamicLibrary::setKDisplay(eDisplay k) {
//	kDisplay = k;
//}
//eDisplay DisplayDynamicLibrary::getKDisplay() const {
//	return kDisplay;
//}
//
//DisplayDynamicLibrary::~DisplayDynamicLibrary() {
//	unloadExternalDisplayLibrary();
//}
//
//bool DisplayDynamicLibrary::hasConstructorLoaded() const {
//	return	newDisplay != nullptr &&
//			dlHandleDisplay != nullptr &&
//			deleteDisplay != nullptr;
//}
