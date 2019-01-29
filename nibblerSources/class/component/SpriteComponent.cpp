
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

SpriteComponent::SpriteComponent(SpriteComponent const &rhs) {
	*this = rhs;
}
