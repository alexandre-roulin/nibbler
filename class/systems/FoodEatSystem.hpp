#pragma once


#include <KINU/SystemsManager.hpp>

class FoodEatSystem : public KINU::System {
	std::mutex mutex;
public:
	virtual void update();
};


