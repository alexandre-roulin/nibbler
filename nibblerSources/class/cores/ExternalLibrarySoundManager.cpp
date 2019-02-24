#include "ExternalLibrarySoundManager.hpp"
#include <dlfcn.h>

ExternalLibrarySoundManager::ExternalLibrarySoundManager() :
	deleteSound(nullptr),
	sound(nullptr),
	newSound(nullptr),
	dlHandleSound(nullptr),
	flag_(0),
	kSound(kSoundSfmlLibrary){
}

void ExternalLibrarySoundManager::switchNextLibrary() {
	unloadExternalSoundLibrary();
	if (kSound == kSoundSfmlLibrary)
		kSound = kSoundSdlLibrary;
	else
		kSound = kSoundSfmlLibrary;
	loadExternalSoundLibrary(kSound);
}

void ExternalLibrarySoundManager::loadExternalSoundLibrary(eSound sound) {

	if (!(dlHandleSound = dlopen(libraryInfo[sound], RTLD_LAZY | RTLD_LOCAL))) {
		dlError("dlopen");
		return;
	}

	if (!(newSound = reinterpret_cast<ISound *(*)() >(
			dlsym(dlHandleSound, "newSound")))) {
		dlError("dlsym");
		return;
	}
	if (!(deleteSound = reinterpret_cast<void (*)(
			ISound *)>(dlsym(dlHandleSound, "deleteSound")))) {
		dlError("dlsym");
		return;
	}
}

void ExternalLibrarySoundManager::constructExternalLibrary() {
	sound = newSound();
}

void ExternalLibrarySoundManager::unloadExternalSoundLibrary() {
	if (sound != nullptr && dlHandleSound != nullptr) {
		if (deleteSound) {
			deleteSound(sound);
			deleteSound = nullptr;
			newSound = nullptr;
			sound = nullptr;
		}
		dlclose(dlHandleSound);
	}
}

void ExternalLibrarySoundManager::dlError(char const *from) {
	std::cerr << "Error " << from << " : " << dlerror() << std::endl;
	unloadExternalSoundLibrary();
}

bool ExternalLibrarySoundManager::hasLibraryLoaded() const {
	return sound != nullptr;
}

eSound ExternalLibrarySoundManager::getKSound() const {
	return kSound;
}

void ExternalLibrarySoundManager::setKSound(eSound kSound) {
	ExternalLibrarySoundManager::kSound = kSound;
}

void ExternalLibrarySoundManager::addNoise(std::string const &path) {
	if (sound && flag_.test(eFlag::kSound))
		sound->addNoise(path);
}
void ExternalLibrarySoundManager::playNoise(eNoise e) const {
	if (sound && flag_.test(eFlag::kSound))
		sound->playNoise(static_cast<int>(e));
}
void ExternalLibrarySoundManager::playMusic(std::string const &path) const {
	if (sound && flag_.test(eFlag::kSound)) {
		sound->setMusic(path.c_str());
		sound->playMusic();
	}
}

ExternalLibrarySoundManager::~ExternalLibrarySoundManager() {
	unloadExternalSoundLibrary();
}
