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
		SfmlSoundException(void) noexcept;

		SfmlSoundException(std::string const &) noexcept;

		const char *what() const noexcept override;

		~SfmlSoundException(void) noexcept = default;

		SfmlSoundException(SfmlSoundException const &src) noexcept;

	private:
		SfmlSoundException &operator=(SfmlSoundException const &rhs) noexcept;

		std::string error_;
	};

	SoundSfml(void) = default;

	~SoundSfml(void) override = default;
	
	void setMusic(std::string const &path) override;

	void playMusic(void) override;

	void addNoise(std::string const &path) override;

	void playNoise(unsigned int index) override;

private:

	void clean_(void);

	sf::Music music_;

	std::vector<std::unique_ptr<sf::SoundBuffer> > noiseBuffer_;

	std::deque<sf::Sound> noiseDeque_;

	SoundSfml &operator=(SoundSfml const &rhs) = delete;

	SoundSfml(SoundSfml const &src) = delete;
};

extern "C" {
ISound *newSound(void);
void deleteSound(ISound *sound);
}
