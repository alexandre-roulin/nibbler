#pragma once

#include <component/PositionComponent.hpp>

class FoodCreation {

public:
	FoodCreation(PositionComponent = PositionComponent(0,0));

	PositionComponent positionComponent_;
};


