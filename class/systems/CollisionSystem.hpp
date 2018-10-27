#pragma once


#include <KNU/systems/System.hpp>

class CollisionSystem : public KNU::System {
public:
	CollisionSystem();

	virtual void update();
};


