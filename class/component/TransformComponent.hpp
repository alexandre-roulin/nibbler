#pragma once

#include <ostream>
#include <KNU/utils/Signature.hpp>
#include <KNU/component/Component.hpp>

struct TransformComponent {
	explicit TransformComponent(int y = 0, int x = 0)
			: y(y), x(x),
			  mask(KNU::Signature(
					  1 << KNU::Component<TransformComponent>::signature())) {

	};

	int y, x;

	KNU::Signature mask;
















	TransformComponent &
	operator=(TransformComponent const &motionComponent) {
		if (this != &motionComponent) {
			y = motionComponent.y;
			x = motionComponent.x;
			mask = motionComponent.mask;
		}
		return *this;
	}

	friend std::ostream &
	operator<<(std::ostream &os, const TransformComponent &component) {
		os << "y: " << component.y << " x: " << component.x << " mask: "
		   << component.mask;
		return os;
	}
};

