#pragma once


#include <KINU/SystemsManager.hpp>
#include <component/PositionComponent.hpp>
#include "nibbler.hpp"

class RenderSystem : public KINU::System {
public:
	RenderSystem();
	virtual void update();
	static int getSprite_(eSprite sprite);
};


