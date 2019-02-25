#pragma once

#include <ISound.hpp>
#include <bitset>
#include "nibbler.hpp"

class ExternalLibrarySoundManager {

public:
	ExternalLibrarySoundManager();
	virtual ~ExternalLibrarySoundManager();
	ExternalLibrarySoundManager &operator=(const ExternalLibrarySoundManager &) = delete;
	ExternalLibrarySoundManager(const ExternalLibrarySoundManager &) = delete;

	void switchNextLibrary();
	void loadExternalSoundLibrary(eSound sound);
	void constructExternalLibrary();
	void unloadExternalSoundLibrary();
	void dlError(char const *from);
	bool hasLibraryLoaded() const;
	eSound getKSound() const;
	void setKSound(eSound kSound);
	void addNoise(std::string const &path);
	void playNoise(eNoise e) const;
	void playMusic(std::string const &path) const;


private:
	static constexpr char libraryInfo[sizeof(eSoundArray) / sizeof(eSound)][PATH_MAX] = {
			{"dynamicLibraries/libsound_sfml.so"}, //[kSoundSfmlLibrary] = 0
			{"dynamicLibraries/libsound_sdl.so"}, //[kSoundSdlLibrary] = 1
	};

	void(*deleteSound)(ISound *);
	ISound *sound;
	ISound *(*newSound)();
	void *dlHandleSound;
	eSound kSound;
};


