#pragma once


class FoodEvent {
public:
	explicit FoodEvent(bool consume = true) : consume(consume) {};
	bool consume;
};


