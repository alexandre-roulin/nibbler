#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <exception>
#include "ISound.hpp"
#include <vector>

class SoundSdl : public ISound {
public:

	class SdlSoundException : public std::exception {
	public:
		SdlSoundException(void) noexcept;

		SdlSoundException(std::string const &) noexcept;

		const char *what() const noexcept override;

		~SdlSoundException(void) noexcept = default;

		SdlSoundException(SdlSoundException const &src) noexcept;

	private:
		SdlSoundException &operator=(SdlSoundException const &rhs) noexcept;

		std::string error_;
	};

	SoundSdl();

	~SoundSdl() override;

	SoundSdl &operator=(SoundSdl const &rhs) = delete;

	SoundSdl(SoundSdl const &src) = delete;


	void setMusic(std::string const &path) override;

	void playMusic() override;
	
	void stopMusic() override;

	void addNoise(std::string const &path) override;

	void playNoise(unsigned int index) override;

private:

	void clean_();

	Mix_Music *music_;
	std::vector<Mix_Chunk *> sound_;
};

extern "C" {
ISound *newInstance(void);
void deleteInstance(ISound *sound);
}
