#include <iostream>
#include "SoundSdl.hpp"

ISound *newSound(void) {
	return (new SoundSdl());
}

void deleteSound(ISound *sound) {
	delete sound;
}

SoundSdl::SoundSdl(void) :
		music_(nullptr) {
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		throw (SoundSdl::SdlSoundException("Audio cant be loaded"));
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		throw (SoundSdl::SdlSoundException("Audio cant be loaded"));
}

SoundSdl::~SoundSdl(void) {
	clean_();
}

void SoundSdl::setMusic(std::string const &path) {
	if (music_) {
		Mix_FreeMusic(music_);
		music_ = nullptr;
	}
	if (!(music_ = Mix_LoadMUS(path.c_str())))
		throw (SoundSdl::SdlSoundException("Background music, cannot be loaded"));
}

void SoundSdl::playMusic() {
	if (Mix_PlayMusic(music_, -1) < 0)
		throw (SoundSdl::SdlSoundException("Cannot playing Background music"));
}

void SoundSdl::stopMusic() {
	if (Mix_HaltMusic() < 0)
		throw (SoundSdl::SdlSoundException("Cannot playing Background music"));
}

void SoundSdl::addNoise(std::string const &path) {
	Mix_Chunk *sound;

	sound = Mix_LoadWAV_RW(SDL_RWFromFile(path.c_str(), "rb"), 1);
	if (sound == NULL)
		throw (SoundSdl::SdlSoundException("Cant load [" + path + "] noise"));
	sound_.push_back(sound);
}

void SoundSdl::playNoise(unsigned int index) {
	if (index >= sound_.size())
		throw (SoundSdl::SdlSoundException("Cant play the noise at [" + std::to_string(index) + "] index"));
	Mix_PlayChannel(-1, sound_[index], 0);
}

void SoundSdl::clean_(void) {
	if (music_) {
		Mix_FreeMusic(music_);
		music_ = nullptr;
	}
	for (auto &it : sound_)
		delete it;
}

SoundSdl::SdlSoundException::SdlSoundException(void) noexcept :
		error_("Error on SoundSdl constructor") {}

SoundSdl::SdlSoundException::SdlSoundException(std::string const &s) noexcept :
		error_(s) {}

SoundSdl::SdlSoundException::SdlSoundException(SoundSdl::SdlSoundException const &src) noexcept :
		error_(src.error_) { error_ = src.error_; }

const char *SoundSdl::SdlSoundException::what() const noexcept { return (error_.c_str()); }
