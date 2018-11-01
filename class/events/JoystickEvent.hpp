#pragma once


#include <nibbler.hpp>
#include <string>
struct JoystickEvent {
	explicit JoystickEvent(eJoystick = NONE, std::string tag_player = "");
	eJoystick joystick;
	std::string tag_player;
};