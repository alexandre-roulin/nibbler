#pragma once


#include <KINU/SystemsManager.hpp>
#include <component/PositionComponent.hpp>
#include "nibbler.hpp"

class RenderSystem : public KINU::System {
public:
	RenderSystem();
	virtual void update();
private:
	static void debugSpriteSnake_(eSprite sprite);
	static int getSpriteSnake_(eSprite sprite);
};


