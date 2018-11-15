#pragma once

#include <string>
#include <KINU/Entity.hpp>
class FoodEvent {
public:
	explicit FoodEvent(KINU::Entity::Id id = 0);
	KINU::Entity::Id id_;
};


