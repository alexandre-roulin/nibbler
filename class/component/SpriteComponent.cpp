
#include "SpriteComponent.hpp"

SpriteComponent::SpriteComponent(int sprite) :
		sprite(sprite) {

}

SpriteComponent &
SpriteComponent::operator=(SpriteComponent const &motionComponent) {
	if (this != &motionComponent) {
		sprite = motionComponent.sprite;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &os, const SpriteComponent &component) {
	os << "sprite: " << component.sprite;
	return os;
}
