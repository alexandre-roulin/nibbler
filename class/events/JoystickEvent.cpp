#include "JoystickEvent.hpp"

JoystickEvent::JoystickEvent(int16_t id, eDirection direction)
		: id(id),
		  direction(direction) {
}

JoystickEvent::JoystickEvent(JoystickEvent const &joystickEvent) {
	*this = joystickEvent;
}

JoystickEvent &JoystickEvent::operator=(JoystickEvent const &joystickEvent) {
	if (this != &joystickEvent) {
		id = joystickEvent.id;
		direction = joystickEvent.direction;
	}
	return *this;
}
