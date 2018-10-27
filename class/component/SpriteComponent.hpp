#pragma once

#include <utility>

#include <ostream>


struct SpriteComponent {

	explicit SpriteComponent(std::string path = "");

	std::string path;

	SpriteComponent &operator=(SpriteComponent const &motionComponent);

	friend std::ostream &
	operator<<(std::ostream &os, const SpriteComponent &component) {
		os << "path: " << component.path;
		return os;
	}
};

