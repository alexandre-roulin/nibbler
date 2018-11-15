#pragma once

#include <KINU/Entity.hpp>
#include <ostream>
#include "PositionComponent.hpp"

struct FollowComponent {
private:
	static std::string const SKIP;
	static std::string const ID_FOLLOWED;
	static std::string const POSITION_COMPONENT;
public:
	friend std::ostream &
	operator<<(std::ostream &os, const FollowComponent &component);

	static std::string const NAME_COMPONENT;

	explicit FollowComponent(KINU::Entity::Id id = 0, bool skip = false);

	explicit FollowComponent(Json::Value);

	KINU::Entity::Id idFollowed;
	PositionComponent positionComponent;
	bool skip;

	Json::Value serializeComponent();
};


