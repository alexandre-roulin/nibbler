#pragma once

#include <ostream>
#include <json/json.h>

struct PositionComponent {
private:
	static std::string const POS_Y;
	static std::string const POS_X;
public:
	static std::string const NAME_COMPONENT;

	explicit PositionComponent(int y = 0, int x = 0);
	PositionComponent(PositionComponent const &);
	PositionComponent(Json::Value);

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

