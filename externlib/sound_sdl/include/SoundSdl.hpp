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
			SdlSoundException(void) throw();
			SdlSoundException(std::string) throw();
			virtual const char* what() const throw();
			~SdlSoundException(void) throw();
			SdlSoundException(SdlSoundException const &src) throw();
		private:
			SdlSoundException &operator=(SdlSoundException const &rhs) throw();
			std::string			_error;
		};

    SoundSdl(void);
    virtual ~SoundSdl(void);

	void		setMusic(char const *path);
	void		playMusic(void);
	void		addNoise(std::string const &path);
	void		playNoise(unsigned int index);

private:

	void			_clean(void);
	void			_error(std::string s);


	Mix_Music						*_music;
	std::vector< Mix_Chunk * >		_sound;

    SoundSdl          &operator=(SoundSdl const &rhs);
    SoundSdl(SoundSdl const &src);
};

extern "C" {
    ISound				*newSound(void);
    void				deleteSound(ISound *sound);
}
