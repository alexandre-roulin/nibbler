#include <iostream>
#include "SoundSfml.hpp"

ISound			*newSound(void)
{
	return (new SoundSfml());
}

void				deleteSound(ISound *sound)
{
	delete sound;
}

SoundSfml::SoundSfml(void)
{
}

SoundSfml::~SoundSfml(void)
{
    this->_clean();
}

void SoundSfml::_error(std::string s = std::string("Error"))
{
    this->_clean();
    throw(SoundSfml::SfmlSoundException(s));
}

void		SoundSfml::setMusic(char const *path)
{
	if (!this->_music.openFromFile(path))
		throw(SoundSfml::SfmlSoundException("Background music, cant be loaded"));
}
void		SoundSfml::playMusic(void)
{
	this->_music.play();
	this->_music.setLoop(true);
}

void		SoundSfml::addNoise(std::string const &path)
{
	this->_noiseBuffer.emplace_back(new sf::SoundBuffer());
	if (!this->_noiseBuffer.back()->loadFromFile(path)) {
		this->_noiseBuffer.pop_back();
		throw(SoundSfml::SfmlSoundException("Cant load [" + path + "] noise"));
	}
}
void		SoundSfml::playNoise(unsigned int index)
{
	//if (index >= this->_noise.size())
	if (index >= this->_noiseBuffer.size())
		throw(SoundSfml::SfmlSoundException("Cant play the noise at " + std::to_string(index) + "index"));
	this->_noiseDeque.emplace_back(sf::Sound());
	this->_noiseDeque.back().setBuffer(*this->_noiseBuffer[index]);
	this->_noiseDeque.back().play();
	
	for (; this->_noiseDeque.front().getStatus() == sf::Sound::Stopped; this->_noiseDeque.pop_front())
	std::cout << "Size: " << this->_noiseDeque.size() << std::endl;
}

void SoundSfml::_clean(void)
{
}

SoundSfml::SfmlSoundException::~SfmlSoundException(void) throw(){}
SoundSfml::SfmlSoundException::SfmlSoundException(void) throw() :
	_error("Error on SoundSfml constructor") {}
SoundSfml::SfmlSoundException::SfmlSoundException(std::string s) throw() :
	_error(s) { }
SoundSfml::SfmlSoundException::SfmlSoundException(SoundSfml::SfmlSoundException const &src) throw() :
	_error(src._error)
	{ this->_error = src._error; }
const char	*SoundSfml::SfmlSoundException::what() const throw()
	{ return (this->_error.c_str()); }
