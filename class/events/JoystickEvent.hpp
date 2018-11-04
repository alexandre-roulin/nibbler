#pragma once


#include <nibbler.hpp>
#include <string>
#include <factory/Factory.hpp>

struct JoystickEvent {

	explicit JoystickEvent(eDirection joystick = NORTH, int16_t id = -1)
			: joystick(joystick),
			  tag_player(Factory::factory_name(HEAD, id)) {
	}
	eDirection joystick;
	std::string tag_player;
};