#include <iostream>
#include "SoundSdl.hpp"

ISound			*newSound(void)
{
	return (new SoundSdl());
}

void				deleteSound(ISound *sound)
{
	delete sound;
}

SoundSdl::SoundSdl(void) :
_music(nullptr)
{
	if(SDL_Init(SDL_INIT_AUDIO) < 0)
		throw(SoundSdl::SdlSoundException("Audio cant be loaded"));
	if(Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1)
		throw(SoundSdl::SdlSoundException("Audio cant be loaded"));
}

SoundSdl::~SoundSdl(void)
{
    this->_clean();
}

void SoundSdl::_error(std::string s = std::string("Error"))
{
    this->_clean();
    throw(SoundSdl::SdlSoundException(s));
}

void		SoundSdl::setMusic(char const *path)
{
	if (this->_music)
	{
		Mix_FreeMusic(this->_music);
		this->_music = nullptr;
	}
	if (!(this->_music = Mix_LoadMUS(path)))
		throw(SoundSdl::SdlSoundException("Background music, cannot be loaded"));
}
void		SoundSdl::playMusic(void)
{
	if (Mix_PlayMusic(this->_music, -1) < 0)
		throw(SoundSdl::SdlSoundException("Cannot playing Background music"));
}

void		SoundSdl::addNoise(std::string const &path)
{
	Mix_Chunk *sound;

	sound = Mix_LoadWAV_RW(SDL_RWFromFile(path.c_str(), "rb"), 1);
	if (sound == NULL)
		throw(SoundSdl::SdlSoundException("Cant load [" + path + "] noise"));
	this->_sound.push_back(sound);
}
void		SoundSdl::playNoise(unsigned int index)
{
	if (index >= this->_sound.size())
		throw(SoundSdl::SdlSoundException("Cant play the noise at [" + std::to_string(index) + "] index"));
	Mix_PlayChannel(-1, this->_sound[index], 0);
}

void SoundSdl::_clean(void)
{
	if (this->_music)
	{
		Mix_FreeMusic(this->_music);
		this->_music = nullptr;
	}
	for (auto &it : this->_sound)
		delete it;
}

SoundSdl::SdlSoundException::~SdlSoundException(void) throw(){}
SoundSdl::SdlSoundException::SdlSoundException(void) throw() :
	_error("Error on SoundSdl constructor") {}
SoundSdl::SdlSoundException::SdlSoundException(std::string s) throw() :
	_error(s) { }
SoundSdl::SdlSoundException::SdlSoundException(SoundSdl::SdlSoundException const &src) throw() :
	_error(src._error)
	{ this->_error = src._error; }
const char	*SoundSdl::SdlSoundException::what() const throw()
	{ return (this->_error.c_str()); }
