#include "CollectComponent.hpp"

CollectComponent::CollectComponent(bool food) : food(food) {}

CollectComponent &CollectComponent::operator=(CollectComponent const &dropComponent) {
	if (this != &dropComponent) {
		food = dropComponent.food;
	}
	return *this;
}
