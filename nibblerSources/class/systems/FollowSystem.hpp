#pragma once


#include <KINU/SystemsManager.hpp>

class FollowSystem : public KINU::System {
public:
	FollowSystem();
	virtual void update();
};


