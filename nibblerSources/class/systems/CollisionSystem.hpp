#pragma once

#include <Univers.hpp>
#include <KINU/SystemsManager.hpp>

class CollisionSystem : public KINU::System {
public:
	CollisionSystem(Univers &univers);
	virtual void update();

private:
	Univers &univers_;
	void createAppleBySnake(KINU::Entity);
	void checkCollision(KINU::Entity entityHead, KINU::Entity entityCheck);
};



