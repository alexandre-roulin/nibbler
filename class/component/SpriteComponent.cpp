
#include "SpriteComponent.hpp"
#include "nibbler.hpp"

SpriteComponent::SpriteComponent(eSprite sprite, ePriority priority) :
		sprite(sprite),
		priority(priority) {

}

SpriteComponent &
SpriteComponent::operator=(SpriteComponent const &sp) {
	if (this != &sp) {
		sprite = sp.sprite;
		priority = sp.priority;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &os, const SpriteComponent &component) {
	os << "eSprite: " << static_cast<int>(component.sprite);
	return os;
}

SpriteComponent::SpriteComponent(SpriteComponent const &rhs) {
	*this = rhs;
}
