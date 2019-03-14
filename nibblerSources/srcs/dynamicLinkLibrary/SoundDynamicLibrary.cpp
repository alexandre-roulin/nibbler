#include "SoundDynamicLibrary.hpp"
#include <boost/filesystem.hpp>
#include <nibbler.hpp>

SoundDynamicLibrary::SoundDynamicLibrary() :
		ADynamicLinkLibrary< ISound, eSound, ISound *(*)() >(kSoundSfmlLibrary),
		noise_(false),
		musique_(false) {
	boost::filesystem::path pathDynamicLibraries(NIBBLER_BUILD_PROJECT_PATH);

	ADynamicLinkLibrary< ISound, eSound, ISound *(*)() >::libraryInfo.reserve(2);
	ADynamicLinkLibrary< ISound, eSound, ISound *(*)() >::libraryInfo.emplace_back("SFML", (pathDynamicLibraries / "dynamicLibraries/libsound_sfml.so").generic_path().c_str(), eSound::kSoundSfmlLibrary);
	ADynamicLinkLibrary< ISound, eSound, ISound *(*)() >::libraryInfo.emplace_back("SDL", (pathDynamicLibraries / "dynamicLibraries/libsound_sdl.so").generic_path().c_str(), eSound::kSoundSdlLibrary);

	setNextKInstance(kSoundSfmlLibrary);
}

void SoundDynamicLibrary::addNoise(std::string const &path) {
	if (instance_)
		instance_->addNoise(path);
}
void SoundDynamicLibrary::playNoise(eNoise e) const {
	if (instance_ && noise_)
		instance_->playNoise(static_cast<int>(e));
}
void SoundDynamicLibrary::playMusic(std::string const &path) const {
	if (instance_ && musique_) {
		instance_->setMusic(path.c_str());
		instance_->playMusic();
	}
}
void SoundDynamicLibrary::stopMusic() {
	if (instance_) {
		instance_->stopMusic();
		musique_ = false;
	}
}

void SoundDynamicLibrary::setMusique(bool b) {
	musique_ = b;
	if (!musique_)
		instance_->stopMusic();
}
void SoundDynamicLibrary::setNoise(bool b) {
	noise_ = b;
}
bool &SoundDynamicLibrary::getMusique() {
	return musique_;
}
bool &SoundDynamicLibrary::getNoise() {
	return noise_;
}