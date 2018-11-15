#include "JoystickEvent.hpp"

JoystickEvent::JoystickEvent(int16_t id, eDirection direction)
		: id(id),
		  direction(direction) {
}