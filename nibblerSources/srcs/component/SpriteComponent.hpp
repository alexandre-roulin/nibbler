#pragma once

#include <utility>

#include <ostream>
#include <nibbler.hpp>

struct SpriteComponent {

	explicit SpriteComponent(
			eSprite sprite = eSprite::kGreen,
			ePriority priority = ePriority ::kNoPriority
					);

	SpriteComponent(SpriteComponent const &);

	eSprite sprite;
	ePriority priority;

	SpriteComponent &operator=(SpriteComponent const &sp);

};

