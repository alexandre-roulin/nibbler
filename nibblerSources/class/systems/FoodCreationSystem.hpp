#pragma once

#include <mutex>
#include <KINU/SystemsManager.hpp>

class FoodCreationSystem: public KINU::System {
public:
	FoodCreationSystem();
	virtual void update();
};


