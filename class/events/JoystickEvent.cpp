#include "JoystickEvent.hpp"

JoystickEvent::JoystickEvent(int16_t id, eDirection direction)
	: tag_player(Factory::factory_name(HEAD, id)),
		  direction(direction) {
//		std::cout << *this << std::endl;
}
