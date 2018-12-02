#include "JoystickEvent.hpp"

JoystickEvent::JoystickEvent(KINU::Entity::ID id, eDirection direction)
		: id(id),
		  direction(direction) {
}