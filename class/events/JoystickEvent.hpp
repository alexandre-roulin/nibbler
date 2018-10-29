#pragma once


#include <nibbler.hpp>

struct JoystickEvent {
	explicit JoystickEvent(eJoystick = NONE);
	eJoystick joystick;
};