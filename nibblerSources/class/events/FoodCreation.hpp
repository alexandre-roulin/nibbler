#pragma once

#include <component/PositionComponent.hpp>

class FoodCreation {

public:
	FoodCreation(PositionComponent = PositionComponent(0,0), bool fromSnake = false);

	PositionComponent positionComponent_;
	bool fromSnake_;
};


