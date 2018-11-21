#pragma once
#include <SFML/Audio.hpp>
#include <exception>
#include "ISound.hpp"
#include <vector>
#include <deque>

class SoundSfml : public ISound {
    public:

		class SfmlSoundException : public std::exception {
		public:
			SfmlSoundException(void) throw();
			SfmlSoundException(std::string) throw();
			virtual const char* what() const throw();
			~SfmlSoundException(void) throw();
			SfmlSoundException(SfmlSoundException const &src) throw();
		private:
			SfmlSoundException &operator=(SfmlSoundException const &rhs) throw();
			std::string			_error;
		};

    SoundSfml(void);
    virtual ~SoundSfml(void);

	void		setMusic(char const *path);
	void		playMusic(void);
	void		addNoise(std::string const &path);
	void		playNoise(unsigned int index);

private:

	void			_clean(void);
	void			_error(std::string s);

	sf::Music								_music;

	std::vector< std::unique_ptr< sf::SoundBuffer> >			_noiseBuffer;
	std::vector< std::unique_ptr< sf::Sound > >					_noise;

	std::deque< sf::Sound >										_noiseDeque;

    SoundSfml          &operator=(SoundSfml const &rhs);
    SoundSfml(SoundSfml const &src);
};

extern "C" {
    ISound				*newSound(void);
    void				deleteSound(ISound *sound);
}
