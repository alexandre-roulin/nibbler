#pragma once


#include <KINU/System.hpp>
#include <component/PositionComponent.hpp>
#include "nibbler.hpp"

class RenderSystem : public KINU::System {
public:
	RenderSystem();
	virtual void update();
private:
	static int getSpriteSnake_(eSprite body, PositionComponent &previous, PositionComponent &actual, PositionComponent &follow);
};


