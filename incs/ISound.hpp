#pragma once

class ISound {
public:
	virtual 			~ISound() {}
	virtual void		setMusic(char const *path) = 0;
	virtual void		playMusic(void) = 0;
	virtual void		addNoise(std::string const &path) = 0;
	virtual void		playNoise(unsigned int index) = 0;
};
