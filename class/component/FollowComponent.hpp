#pragma once


#include <KNU/entities/Entity.hpp>

struct FollowComponent {
	explicit FollowComponent(KNU::Entity::ID id = 0);
	KNU::Entity::ID _idFollowed;
	FollowComponent &operator=(FollowComponent const &);
};


