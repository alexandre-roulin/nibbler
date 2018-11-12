#pragma once


#include <KINU/System.hpp>

class FoodSystem: public KINU::System {
public:
	FoodSystem();
	void createFood();
	virtual void update();
};


