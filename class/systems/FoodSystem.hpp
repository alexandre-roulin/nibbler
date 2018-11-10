#pragma once


#include <KNU/systems/System.hpp>

class FoodSystem: public KNU::System {
public:
	FoodSystem();
	void createFood();
	virtual void update();
};


