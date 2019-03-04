#include "ExternalLibrarySoundManager.hpp"
#include <dlfcn.h>
#include <gui/Gui.hpp>

ExternalLibrarySoundManager::ExternalLibrarySoundManager(Univers &univers) :
	deleteSound(nullptr),
	sound(nullptr),
	newSound(nullptr),
	dlHandleSound(nullptr),
	kSound(kSoundSfmlLibrary),
	noise_(false),
	musique_(false),
	univers_(univers) {
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
		dlError();
		return;
	}

	if (!(newSound = reinterpret_cast<ISound *(*)() >(
			dlsym(dlHandleSound, "newSound")))) {
		dlError();
		return;
	}
	if (!(deleteSound = reinterpret_cast<void (*)(
			ISound *)>(dlsym(dlHandleSound, "deleteSound")))) {
		dlError();
		return;
	}
}

void ExternalLibrarySoundManager::constructExternalLibrary() {
	if (newSound)
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

void ExternalLibrarySoundManager::dlError() {
	std::unique_ptr<Gui> &gui = univers_.getGui_();
	if (gui)
		gui->addMessageChat(eColorLog::kRed, dlerror());
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
	if (sound)
		sound->addNoise(path);
}
void ExternalLibrarySoundManager::playNoise(eNoise e) const {
	if (sound && noise_)
		sound->playNoise(static_cast<int>(e));
}
void ExternalLibrarySoundManager::playMusic(std::string const &path) const {
	if (sound && musique_) {
		sound->setMusic(path.c_str());
		sound->playMusic();
	}
}
void ExternalLibrarySoundManager::stopMusic() const {
	if (sound) {
		sound->stopMusic();
	}
}

void ExternalLibrarySoundManager::setMusique(bool b) {
	musique_ = b;
	if (!musique_)
		sound->stopMusic();
}
void ExternalLibrarySoundManager::setNoise(bool b) {
	noise_ = b;
}
bool &ExternalLibrarySoundManager::getMusique() {
	return musique_;
}
bool &ExternalLibrarySoundManager::getNoise() {
	return noise_;
}

ExternalLibrarySoundManager::~ExternalLibrarySoundManager() {
	unloadExternalSoundLibrary();
}
