#pragma once

#include <KINU/Entity.hpp>
#include <ostream>
#include "PositionComponent.hpp"

struct FollowComponent {
	explicit FollowComponent(KINU::Entity::ID id = 0, bool skip = false);

	KINU::Entity::ID	 idFollowed;
	PositionComponent positionComponent;
	bool skip;

};


