#pragma once


#include <nibbler.hpp>

struct JoystickComponent {
public:

	explicit JoystickComponent(eDirection direction = NORTH);

	eDirection direction;
};


