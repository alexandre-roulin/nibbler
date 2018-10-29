#include <SFML/Audio.hpp>
#include <unistd.h>
#include <iostream>
#include "SoundSdl.hpp"

int main()
{
	{
		SoundSdl ssdl;
		(void)ssdl;
		/*
	    sf::SoundBuffer buffer;
	    if (!buffer.loadFromFile("../commun/yes-2.wav"))
	        return -1;
		sf::Sound sound;
		sound.setBuffer(buffer);
		sound.play();
		*/
		ssdl.setMusic("zelda.ogg");
		ssdl.playMusic();
		ssdl.addNoise("../commun/yes-2.wav");
		for (long int i = 0; i < 10000000000; i++)
		{
			if (!(i % 100000000))
				ssdl.playNoise(0);
		}
	}
    return 0;
}
