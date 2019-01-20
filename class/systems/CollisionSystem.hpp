#pragma once


#include <KINU/SystemsManager.hpp>

class CollisionSystem : public KINU::System {
public:
	CollisionSystem();
	virtual void update();

private:
	void createAppleBySnake(KINU::Entity);
	void checkCollision(KINU::Entity entityHead, KINU::Entity entityCheck);
};



