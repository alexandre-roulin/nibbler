#include <iostream>
#include "CollisionComponent.hpp"

std::string const CollisionComponent::NAME_COMPONENT = "collisionComponent";
std::string const CollisionComponent::IS_WALL = "isWall";

CollisionComponent::CollisionComponent(bool is_wall)
		: isWall(is_wall) {

}

Json::Value CollisionComponent::serializeComponent() {
	Json::Value component;
	component[CollisionComponent::NAME_COMPONENT][CollisionComponent::IS_WALL] = isWall;
	return component;
}

CollisionComponent::CollisionComponent(Json::Value json) {
	assert(json[IS_WALL].isBool());
	isWall = json.get(IS_WALL, true).asBool();
}
