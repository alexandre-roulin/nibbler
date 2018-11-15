#pragma once


#include <KINU/System.hpp>

class CollisionSystem : public KINU::System {
public:
	CollisionSystem();
	virtual void update();

private:
	void checkCollision(KINU::Entity &entityHead, KINU::Entity &entityCheck);
};



