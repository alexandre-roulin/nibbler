#pragma once


#include <nibbler.hpp>
#include <string>
#include <factory/Factory.hpp>
#include <ostream>

struct JoystickEvent {

	JoystickEvent(int16_t id = -1, eDirection direction = NORTH);

	JoystickEvent(JoystickEvent const &);

	JoystickEvent &operator=(JoystickEvent const &);

	friend std::ostream &
	operator<<(std::ostream &os, const JoystickEvent &event) {
		os << "direction: " << event.direction << " tag_player: "
		   << event.id;
		return os;
	}

	int16_t id;
	eDirection direction;
};