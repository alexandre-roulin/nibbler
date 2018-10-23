//
// Created by Alexandre ROULIN on 21/10/2018.
//

#ifndef NIBBLER_MOTIONCOMPONENT_HPP
#define NIBBLER_MOTIONCOMPONENT_HPP

#include "Component.hpp"
#include <nibbler.hpp>
#include <ostream>

struct MotionComponent : Component<MotionComponent> {
	MotionComponent() : Component(MOTION_COMPONENT), _speed(0), _dir(NONE) {}
	MotionComponent(int _speed, eDirection _dir) : Component(MOTION_COMPONENT), _speed(_speed), _dir(_dir) {}

	int _speed;
	eDirection _dir;
};

std::ostream &operator<<(std::ostream &os, const MotionComponent &mc) {
	os << " _speed : " << mc._speed << " _dir : " << mc._dir;
	return os;
}

#endif //NIBBLER_MOTIONCOMPONENT_HPP
