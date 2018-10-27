#pragma once


#include <KNU/entities/Entity.hpp>

struct FollowComponent {
	explicit FollowComponent(KNU::Entity::ID id = 0, bool skip = false);
	KNU::Entity::ID _idFollowed;
	bool skip;
	FollowComponent &operator=(FollowComponent const &);
};


