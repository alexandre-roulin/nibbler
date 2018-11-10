#pragma once


#include <nibbler.hpp>
#include <string>
#include <factory/Factory.hpp>
#include <ostream>

struct JoystickEvent {

	explicit JoystickEvent(int16_t id = -1, eDirection direction = NORTH);
	JoystickEvent(JoystickEvent const &);
	JoystickEvent &operator=(JoystickEvent const &);
	friend std::ostream &
	operator<<(std::ostream &os, const JoystickEvent &event) {
		os << "direction: " << event.direction << " tag_player: "
		   << event.tag_player;
		return os;
	}

	eDirection direction;
	std::string tag_player;
};