#pragma once

#include <ISound.hpp>
#include <bitset>
#include "nibbler.hpp"

class Univers;

class ExternalLibrarySoundManager {

public:
	ExternalLibrarySoundManager(Univers &univers);
	virtual ~ExternalLibrarySoundManager();
	ExternalLibrarySoundManager &operator=(const ExternalLibrarySoundManager &) = delete;
	ExternalLibrarySoundManager(const ExternalLibrarySoundManager &) = delete;

	void switchNextLibrary();
	void loadExternalSoundLibrary(eSound sound);
	void constructExternalLibrary();
	void unloadExternalSoundLibrary();
	void dlError();
	bool hasLibraryLoaded() const;
	void addNoise(std::string const &path);
	void playNoise(eNoise e) const;
	void playMusic(std::string const &path) const;
	void stopMusic() const;
	void setMusique(bool b);
	void setNoise(bool b);
	bool &getMusique();
	bool &getNoise();
	ISound *getSound() const;
	eSound getKSound() const;
	void setKSound(eSound kSound);

	static constexpr LibraryInfo< eSound > libraryInfo[sizeof(eSoundArray) / sizeof(eSound)] = {
			{"SFML", "dynamicLibraries/libsound_sfml.so", eSound::kSoundSfmlLibrary}, //[kDisplaySfmlLibrary] = 0
			{"SDL", "dynamicLibraries/libsound_sdl.so", eSound::kSoundSdlLibrary}, //[kDisplaySfmlLibrary] = 0
	};

private:

	void(*deleteSound)(ISound *);
	ISound *sound;
	ISound *(*newSound)();
	void *dlHandleSound;
	eSound kSound;
	bool noise_;
	bool musique_;
	Univers &univers_;
};


