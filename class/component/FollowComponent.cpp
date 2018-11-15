#include "FollowComponent.hpp"
#include <cassert>
#include <iostream>

std::string const FollowComponent::NAME_COMPONENT = "followComponent";
std::string const FollowComponent::SKIP = "skip";
std::string const FollowComponent::ID_FOLLOWED = "idFollowed";
std::string const FollowComponent::POSITION_COMPONENT = "positionComponent";

FollowComponent::FollowComponent(KINU::Entity::Id id, bool skip)
		: idFollowed(id),
		  skip(skip),
		  positionComponent(PositionComponent()){
}

Json::Value FollowComponent::serializeComponent() {
	Json::Value component;
	component[SKIP] = skip;
	component[ID_FOLLOWED] = idFollowed;
	component[POSITION_COMPONENT] = positionComponent.serializeComponent();
	return component;
}

FollowComponent::FollowComponent(Json::Value json) {
	assert(json[SKIP].isBool());
	assert(json[ID_FOLLOWED].isInt());
	assert(json[POSITION_COMPONENT].isObject());

	skip = json.get(SKIP, false).asBool();
	idFollowed = json.get(ID_FOLLOWED, -1).asInt();
	positionComponent = PositionComponent(json.get(POSITION_COMPONENT, {0, 0}));

}

std::ostream &operator<<(std::ostream &os, const FollowComponent &component) {
	os << "idFollowed: " << component.idFollowed << " positionComponent: "
	   << component.positionComponent << " skip: " << component.skip;
	return os;
}
