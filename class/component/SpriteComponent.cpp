#include <KNU/utils/Signature.hpp>
#include "SpriteComponent.hpp"

SpriteComponent::SpriteComponent(std::string path)
		: path(std::move(path)) {

}

SpriteComponent &
SpriteComponent::operator=(SpriteComponent const &motionComponent) {
	if (this != &motionComponent) {
		path = motionComponent.path;
	}
	return *this;
}
