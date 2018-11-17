#pragma once

#include <utility>

#include <ostream>
#include <nibbler.hpp>

struct SpriteComponent {

	explicit SpriteComponent(eSprite sprite = eSprite::GREEN);

	eSprite sprite;

	SpriteComponent &operator=(SpriteComponent const &motionComponent);

	friend std::ostream &
	operator<<(std::ostream &os, const SpriteComponent &component);
};

