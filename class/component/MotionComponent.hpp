#pragma once

#include <ostream>
#include <nibbler.hpp>
#include "KNU/component/Component.hpp"
#include <KNU/utils/Signature.hpp>


class MotionComponent {
public:
	explicit MotionComponent(eDirection direction = NONE,
							 unsigned int speed = 1)
			: direction(direction), speed(speed),
			  mask(KNU::Signature(
					  1 << KNU::Component<MotionComponent>::signature())) {};

	eDirection direction;

	unsigned int speed;

	KNU::Signature mask;

	MotionComponent &operator=(MotionComponent const &motionComponent);

	friend std::ostream &
	operator<<(std::ostream &os, const MotionComponent &component);
};

//	std::ostream &
//	operator<<(std::ostream &os, const TransformComponent &component) {
//		os << "y: " << component.y << " x: " << component.x;
//		return os;
//	}


