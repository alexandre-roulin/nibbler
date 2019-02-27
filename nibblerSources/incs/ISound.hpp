#pragma once

#include <string>

class ISound {
public:
	virtual ~ISound() {}

	virtual void setMusic(std::string const &path) = 0;

	virtual void playMusic() = 0;

	virtual void stopMusic() = 0;

	virtual void addNoise(std::string const &path) = 0;

	virtual void playNoise(unsigned int index) = 0;
};
