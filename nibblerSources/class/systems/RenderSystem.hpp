#pragma once


#include <component/PositionComponent.hpp>
#include "nibbler.hpp"
#include <cores/Univers.hpp>
#include <KINU/SystemsManager.hpp>

class RenderSystem : public KINU::System {
public:
	RenderSystem(Univers &univers_);
	virtual void update();
private:
	Univers &univers_;
};


