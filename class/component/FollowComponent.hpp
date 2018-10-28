#pragma once


#include <KNU/entities/Entity.hpp>
#include "PositionComponent.hpp"

struct FollowComponent {
private:
	static std::string const NAME_COMPONENT;
	static std::string const SKIP;
	static std::string const ID_FOLLOWED;
	static std::string const POSITION_COMPONENT;


public:
	explicit FollowComponent(KNU::Entity::ID id = 0, bool skip = false);
	KNU::Entity::ID _idFollowed;
	PositionComponent positionComponent;
	bool skip;
	Json::Value serializeComponent();
	FollowComponent &operator=(FollowComponent const &);
};


