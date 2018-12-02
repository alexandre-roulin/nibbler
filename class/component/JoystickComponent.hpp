#pragma once


#include <nibbler.hpp>

struct JoystickComponent {
public:

	explicit JoystickComponent(eDirection direction = NORTH, std::string tag = "");

	eDirection direction;
	std::string tag;
};


