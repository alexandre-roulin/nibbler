#pragma once

#include "ADynamicLinkLibrary.hpp"
#include <ISound.hpp>

enum eSound {
	kSoundSfmlLibrary = 0,
	kSoundSdlLibrary
};

enum class eNoise {
	kWelcomeSound = 0,
	kReadySound = 2,
	kResizeSound = 0,
	kFoodSound = 3,
	kDeathSound = 4
};

class SoundDynamicLibrary : public ADynamicLinkLibrary< ISound, eSound, ISound *(*)() > {
public:
	SoundDynamicLibrary();
	virtual ~SoundDynamicLibrary() = default;
	SoundDynamicLibrary &operator=(const SoundDynamicLibrary &) = delete;
	SoundDynamicLibrary(const SoundDynamicLibrary &) = delete;

	void addNoise(std::string const &path);
	void playNoise(eNoise e) const;
	void playMusic(std::string const &path) const;
	void stopMusic();
	void setMusique(bool b);
	void setNoise(bool b);
	bool &getMusique();
	bool &getNoise();

private:
	bool noise_;
	bool musique_;
};
