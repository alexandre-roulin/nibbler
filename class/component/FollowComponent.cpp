#include "FollowComponent.hpp"

FollowComponent::FollowComponent(KINU::Entity::ID id, bool skip)
		: idFollowed(id),
		  skip(skip),
		  positionComponent(PositionComponent()){
}