#pragma once

#include <mutex>
#include <KINU/SystemsManager.hpp>

class FoodCreationSystem: public KINU::System {
public:
	FoodCreationSystem(unsigned int mapSize);
	virtual void update();

protected:
	int mapSize_;
};


