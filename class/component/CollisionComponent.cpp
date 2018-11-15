#include <cassert>
#include <iostream>
#include "CollisionComponent.hpp"
std::string const CollisionComponent::NAME_COMPONENT = "collisionComponent";
std::string const CollisionComponent::IS_WALL = "isWall";

CollisionComponent::CollisionComponent() {

}

Json::Value CollisionComponent::serializeComponent() {
	return Json::Value();
}

CollisionComponent::CollisionComponent(Json::Value json) {
}
