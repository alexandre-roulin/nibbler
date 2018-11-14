#pragma once


#include <KINU/System.h>

class FoodSystem: public Mix::System {
public:
	FoodSystem();
	void createFood();
	virtual void update();
};


