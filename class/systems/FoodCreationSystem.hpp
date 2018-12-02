#pragma once


#include <KINU/SystemsManager.hpp>

class FoodCreationSystem: public KINU::System {
public:
	FoodCreationSystem();

	virtual void update();
};


