#pragma once

#include <utility>

#include <ostream>


struct SpriteComponent {

	explicit SpriteComponent(int sprite = 0);

	int sprite;

	SpriteComponent &operator=(SpriteComponent const &motionComponent);

	friend std::ostream &
	operator<<(std::ostream &os, const SpriteComponent &component);
};

