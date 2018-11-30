#pragma once

#include <KINU/Entity.hpp>
#include <ostream>
#include "PositionComponent.hpp"

struct FollowComponent {
	explicit FollowComponent(KINU::Entity::Id id = 0, bool skip = false);

	KINU::Entity::Id idFollowed;
	PositionComponent positionComponent;
	bool skip;

};


