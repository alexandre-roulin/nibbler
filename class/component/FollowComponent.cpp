#include "FollowComponent.hpp"

FollowComponent::FollowComponent(KNU::Entity::ID id)
	:_idFollowed(id) {

}

FollowComponent &FollowComponent::operator=(FollowComponent const &followComponent) {
	if (this != &followComponent) {
		_idFollowed = followComponent._idFollowed;
	}
	return *this;
}
