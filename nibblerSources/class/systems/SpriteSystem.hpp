#pragma once


#include <KINU/SystemsManager.hpp>
#include "nibbler.hpp"

class SpriteSystem : public KINU::System {
public:
	SpriteSystem(Univers &univers);
	virtual void update();
	static eSprite spriteDirection(PositionComponent &actual, PositionComponent &follow);

private:
	Univers &univers_;
};


