#pragma once

#include <mutex>
#include <KINU/SystemsManager.hpp>

class FoodCreationSystem: public KINU::System {
public:
	FoodCreationSystem(unsigned int mapSize);
	FoodCreationSystem() = delete;
	~FoodCreationSystem() = default;
	FoodCreationSystem &operator=(const FoodCreationSystem &) = delete;
	FoodCreationSystem(const FoodCreationSystem &) = delete;

	virtual void update();

protected:
	int mapSize_;
};


