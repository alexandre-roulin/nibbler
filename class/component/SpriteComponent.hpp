#pragma once
#include <utility>

#include <ostream>


	struct SpriteComponent {

		explicit SpriteComponent(std::string path = "")
				: path(std::move(path)),
				  mask(KNU::Signature(
						  1 << KNU::Component<SpriteComponent>::signature())) {};

		std::string path;
		KNU::Signature mask;

		SpriteComponent &operator=(SpriteComponent const &motionComponent) {
			if (this != &motionComponent) {
				mask = motionComponent.mask;
			}
			return *this;
		}

		friend std::ostream &
		operator<<(std::ostream &os, const SpriteComponent &component) {
			os << "path: " << component.path << " mask: " << component.mask;
			return os;
		}
	};
