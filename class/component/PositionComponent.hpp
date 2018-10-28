#pragma once

#include <ostream>
#include <KNU/utils/Signature.hpp>
#include <KNU/component/Component.hpp>
#include <json/json.h>

struct PositionComponent {
	static std::string const NAME_COMPONENT;
	static std::string const POS_Y;
	static std::string const POS_X;


	explicit PositionComponent(int y = 0, int x = 0);
	PositionComponent(PositionComponent const &);

	int y, x;
	Json::Value serializeComponent();

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

