#pragma once


#include <nibbler.hpp>
#include <string>
#include <factory/Factory.hpp>
#include <ostream>

struct JoystickEvent {

	JoystickEvent(KINU::Entity::Id id = 0, eDirection direction = NORTH);
	KINU::Entity::Id id;
	eDirection direction;
};