#include "JoystickEvent.hpp"

JoystickEvent::JoystickEvent(int16_t id, eDirection direction)
	: tag_player(Factory::factory_name(HEAD, id)),
		  direction(direction) {
//		std::cout << *this << std::endl;
}

JoystickEvent::JoystickEvent(JoystickEvent const &joystickEvent) {
	*this = joystickEvent;
}

JoystickEvent &JoystickEvent::operator=(JoystickEvent const &joystickEvent) {
	if (this != &joystickEvent) {
		tag_player = joystickEvent.tag_player;
		direction = joystickEvent.direction;
	}
	return *this;
}
