#pragma once


#include <nibbler.hpp>
#include <string>
#include <ostream>
#include <KINU/Entity.hpp>

struct JoystickEvent {

	JoystickEvent(KINU::Entity::ID id = 0, eDirection direction = NORTH);
	KINU::Entity::ID id;
	eDirection direction;
};