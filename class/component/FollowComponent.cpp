#include "FollowComponent.hpp"

FollowComponent::FollowComponent(KNU::Entity::ID id, bool skip)
		: _idFollowed(id),
		  skip(skip) {

}

FollowComponent &
FollowComponent::operator=(FollowComponent const &followComponent) {
	if (this != &followComponent) {
		_idFollowed = followComponent._idFollowed;
		skip= followComponent.skip;
	}
	return *this;
}
