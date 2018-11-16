#include "JoystickEvent.hpp"

JoystickEvent::JoystickEvent(KINU::Entity::Id id, eDirection direction)
		: id(id),
		  direction(direction) {
}