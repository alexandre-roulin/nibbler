#pragma once

#include <ostream>
#include <json/json.h>

struct PositionComponent {
public:

	explicit PositionComponent(int x = 0, int y = 0);
	PositionComponent(PositionComponent const &);

	int y, x;


	PositionComponent &
	operator=(PositionComponent const &motionComponent);

	bool operator==(const PositionComponent &rhs) const;

	bool operator!=(const PositionComponent &rhs) const;

};

