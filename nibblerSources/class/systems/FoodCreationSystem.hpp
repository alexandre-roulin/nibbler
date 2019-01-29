#pragma once

#include <mutex>
#include <KINU/SystemsManager.hpp>

class FoodCreationSystem: public KINU::System {
private:
	std::mutex mutex;
public:
	FoodCreationSystem();

	virtual void update();
};


