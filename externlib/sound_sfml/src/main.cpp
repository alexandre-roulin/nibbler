#include <SFML/Audio.hpp>
#include <unistd.h>
#include <iostream>
#include "SoundSfml.hpp"

int main()
{
	SoundSfml ssfml;
	(void)ssfml;
	/*
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("../commun/yes-2.wav"))
        return -1;
	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.play();
	*/
	ssfml.setMusic("zelda.ogg");
	ssfml.playMusic();
	ssfml.addNoise("../commun/yes-2.wav");
	for (long int i = 0; i < 10000000000; i++)
	{
		if (!(i % 10000000))
			ssfml.playNoise(0);
	}
    return 0;
}
