#include "FollowComponent.hpp"
#include <cassert>
#include <iostream>

FollowComponent::FollowComponent(KINU::Entity::Id id, bool skip)
		: idFollowed(id),
		  skip(skip),
		  positionComponent(PositionComponent()){
}