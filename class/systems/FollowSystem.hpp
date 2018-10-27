#pragma once


#include <KNU/systems/System.hpp>

class FollowSystem : public KNU::System {
public:
	FollowSystem();
	virtual void update();
};


