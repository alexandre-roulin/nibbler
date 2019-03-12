#pragma once

#include <component/PositionComponent.hpp>

struct FoodCreation {
	FoodCreation(PositionComponent = PositionComponent(0,0), bool fromSnake = false);

	PositionComponent positionComponent_;
	bool fromSnake_;
};


