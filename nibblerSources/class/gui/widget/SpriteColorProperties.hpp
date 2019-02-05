#pragma once

#include "Sprite.hpp"
#include <SFML/Graphics.hpp>
#include <string>

struct SpriteColorProperties {
	eSprite color;
	sf::Texture sprite;
	std::string name;
};