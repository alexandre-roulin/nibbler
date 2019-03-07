//#include "SoundDynamicLibrary.hpp"
//#include <dlfcn.h>
//#include <gui/Gui.hpp>
//
//SoundDynamicLibrary::SoundDynamicLibrary(Univers &univers) :
//	deleteSound(nullptr),
//	sound(nullptr),
//	newSound(nullptr),
//	dlHandleSound(nullptr),
//	kSound(kSoundSfmlLibrary),
//	noise_(false),
//	musique_(false),
//	univers_(univers) {
//}
//
//void SoundDynamicLibrary::switchNextLibrary() {
//	unloadExternalSoundLibrary();
//	if (kSound == kSoundSfmlLibrary)
//		kSound = kSoundSdlLibrary;
//	else
//		kSound = kSoundSfmlLibrary;
//	loadExternalSoundLibrary(kSound);
//}
//
//void SoundDynamicLibrary::loadExternalSoundLibrary(eSound sound) {
//
//	if (!(dlHandleSound = dlopen(libraryInfo[sound].path, RTLD_LAZY | RTLD_LOCAL))) {
//		dlError();
//		return;
//	}
//
//	if (!(newSound = reinterpret_cast<ISound *(*)() >(
//			dlsym(dlHandleSound, "newSound")))) {
//		dlError();
//		return;
//	}
//	if (!(deleteSound = reinterpret_cast<void (*)(
//			ISound *)>(dlsym(dlHandleSound, "deleteSound")))) {
//		dlError();
//		return;
//	}
//}
//
//void SoundDynamicLibrary::constructExternalLibrary() {
//	if (newSound)
//		sound = newSound();
//}
//
//void SoundDynamicLibrary::unloadExternalSoundLibrary() {
//	if (sound != nullptr && dlHandleSound != nullptr) {
//		if (deleteSound) {
//			deleteSound(sound);
//			deleteSound = nullptr;
//			newSound = nullptr;
//			sound = nullptr;
//		}
//		dlclose(dlHandleSound);
//	}
//}
//
//void SoundDynamicLibrary::dlError() {
//	std::unique_ptr<Gui> &gui = univers_.getGui_();
//	if (gui)
//		gui->addMessageChat(eColorLog::kRed, dlerror());
//	unloadExternalSoundLibrary();
//}
//
//bool SoundDynamicLibrary::hasLibraryLoaded() const {
//	return sound != nullptr;
//}
//
//void SoundDynamicLibrary::addNoise(std::string const &path) {
//	if (sound)
//		sound->addNoise(path);
//}
//void SoundDynamicLibrary::playNoise(eNoise e) const {
//	if (sound && noise_)
//		sound->playNoise(static_cast<int>(e));
//}
//void SoundDynamicLibrary::playMusic(std::string const &path) const {
//	if (sound && musique_) {
//		sound->setMusic(path.c_str());
//		sound->playMusic();
//	}
//}
//void SoundDynamicLibrary::stopMusic() const {
//	if (sound) {
//		sound->stopMusic();
//	}
//}
//
//void SoundDynamicLibrary::setMusique(bool b) {
//	musique_ = b;
//	if (!musique_)
//		sound->stopMusic();
//}
//void SoundDynamicLibrary::setNoise(bool b) {
//	noise_ = b;
//}
//bool &SoundDynamicLibrary::getMusique() {
//	return musique_;
//}
//bool &SoundDynamicLibrary::getNoise() {
//	return noise_;
//}
//
//ISound *SoundDynamicLibrary::getSound() const {
//	return sound;
//}
//
//void SoundDynamicLibrary::setKSound(eSound k) {
//	kSound = k;
//}
//eSound SoundDynamicLibrary::getKSound() const {
//	return kSound;
//}
//
//SoundDynamicLibrary::~SoundDynamicLibrary() {
//	unloadExternalSoundLibrary();
//}
