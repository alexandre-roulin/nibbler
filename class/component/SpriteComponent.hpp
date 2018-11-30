#pragma once

#include <utility>

#include <ostream>
#include <nibbler.hpp>

struct SpriteComponent {

	explicit SpriteComponent(
			eSprite sprite = eSprite::GREEN,
			ePriority priority = ePriority ::NO_PRIORITY
					);

	SpriteComponent(SpriteComponent const &);

	eSprite sprite;
	ePriority priority;

	SpriteComponent &operator=(SpriteComponent const &sp);

};

