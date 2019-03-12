#include "FollowComponent.hpp"

FollowComponent::FollowComponent(KINU::Entity::ID id, bool skip)
		: idFollowed(id),
		  positionComponent(PositionComponent()),
		  skip(skip) {
}