#pragma once

#include <ostream>
#include <KNU/utils/Signature.hpp>
#include <KNU/component/Component.hpp>

struct PositionComponent {
	explicit PositionComponent(int y = 0, int x = 0);
	PositionComponent(PositionComponent const &);
	int y, x;

	PositionComponent &
	operator=(PositionComponent const &motionComponent) {
		if (this != &motionComponent) {
			y = motionComponent.y;
			x = motionComponent.x;
		}
		return *this;
	}

	bool operator==(const PositionComponent &rhs) const;

	bool operator!=(const PositionComponent &rhs) const;

	friend std::ostream &
	operator<<(std::ostream &os, const PositionComponent &component) {
		os << "y: " << component.y << " x: " << component.x;
		return os;
	}
};

