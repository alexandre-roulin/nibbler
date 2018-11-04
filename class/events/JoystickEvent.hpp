#pragma once


#include <nibbler.hpp>
#include <string>
#include <factory/Factory.hpp>

struct JoystickEvent {

	explicit JoystickEvent(int16_t id = -1, eDirection direction = NORTH)
			: tag_player(Factory::factory_name(HEAD, id)),
			  direction(direction) {
	}

	eDirection direction;
	std::string tag_player;
};