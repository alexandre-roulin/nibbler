#pragma once


#include <KINU/SystemsManager.hpp>

class FoodEatSystem : public KINU::System {
public:
	FoodEatSystem() = default;
	~FoodEatSystem() = default;
	FoodEatSystem &operator=(const FoodEatSystem &) = delete;
	FoodEatSystem(const FoodEatSystem &) = delete;
	virtual void update();
};


