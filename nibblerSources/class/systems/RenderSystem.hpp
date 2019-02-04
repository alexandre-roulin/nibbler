#pragma once


#include <KINU/SystemsManager.hpp>
#include <component/PositionComponent.hpp>
#include "nibbler.hpp"

class RenderSystem : public KINU::System {
public:
	RenderSystem(Univers &univers_);
	virtual void update();
	static int getSprite_(eSprite sprite);

private:
	Univers &univers_;
};


