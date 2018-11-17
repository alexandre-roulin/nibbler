#pragma once


#include <KINU/System.hpp>
#include "nibbler.hpp"

class PositionComponent;

class SpriteSystem : public KINU::System {
public:
	SpriteSystem();
	virtual void update();
	static eSprite spriteDirection(PositionComponent &actual, PositionComponent &follow);
};


