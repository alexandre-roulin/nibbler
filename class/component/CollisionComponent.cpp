#include <cassert>
#include <iostream>
#include "CollisionComponent.hpp"

CollisionComponent::CollisionComponent() {

}

std::ostream &
operator<<(std::ostream &os, const CollisionComponent &component) {
	return os;
}
