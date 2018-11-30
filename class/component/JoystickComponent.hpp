#pragma once


#include <nibbler.hpp>
#include <json/json.h>

struct JoystickComponent {
public:

	explicit JoystickComponent(eDirection direction = NORTH, std::string tag = "");

	eDirection direction;
	std::string tag;
};


