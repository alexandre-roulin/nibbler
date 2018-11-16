
#include "SpriteComponent.hpp"
#include "nibbler.hpp"

SpriteComponent::SpriteComponent(eSprite sprite) :
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
	os << "eSprite: " << static_cast<int>(component.sprite);
	return os;
}
