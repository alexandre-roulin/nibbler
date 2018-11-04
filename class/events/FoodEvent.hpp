#pragma once

#include <string>

class FoodEvent {
public:
	explicit FoodEvent(std::string tail, std::string group)
			: tag_tail(tail),
			  tag_group(group) {

	};
	std::string tag_tail;
	std::string tag_group;
};


