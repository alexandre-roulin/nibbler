#include <iostream>
#include "SoundSfml.hpp"

ISound *newInstance(void) {
	return (new SoundSfml());
}

void deleteInstance(ISound *sound) {
	delete sound;
}

void SoundSfml::setMusic(std::string const &path) {
	if (!music_.openFromFile(path))
		throw (SoundSfml::SfmlSoundException("Background music, cant be loaded"));
}

void SoundSfml::playMusic(void) {
	music_.play();
	music_.setLoop(true);
}

void SoundSfml::stopMusic(void) {
	music_.stop();
}

void SoundSfml::addNoise(std::string const &path) {
	noiseBuffer_.emplace_back(new sf::SoundBuffer());
	if (!noiseBuffer_.back()->loadFromFile(path)) {
		noiseBuffer_.pop_back();
		throw (SoundSfml::SfmlSoundException("Cant load [" + path + "] noise"));
	}
}

void SoundSfml::playNoise(unsigned int index) {
	if (index >= noiseBuffer_.size())
		throw (SoundSfml::SfmlSoundException("Cant play the noise at " + std::to_string(index) + "index"));
	noiseDeque_.emplace_back(sf::Sound());
	noiseDeque_.back().setBuffer(*noiseBuffer_[index]);
	noiseDeque_.back().play();

	for (; noiseDeque_.front().getStatus() == sf::Sound::Stopped; noiseDeque_.pop_front())
		std::cout << "Size: " << noiseDeque_.size() << std::endl;
}

SoundSfml::SfmlSoundException::SfmlSoundException(void) noexcept :
		error_("Error on SoundSfml constructor") {}

SoundSfml::SfmlSoundException::SfmlSoundException(std::string const &s) noexcept :
		error_(s) {}

SoundSfml::SfmlSoundException::SfmlSoundException(SoundSfml::SfmlSoundException const &src) noexcept :
		error_(src.error_) { error_ = src.error_; }

const char *SoundSfml::SfmlSoundException::what() const noexcept { return (error_.c_str()); }
