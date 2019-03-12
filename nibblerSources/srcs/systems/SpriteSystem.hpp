#pragma once


#include <KINU/SystemsManager.hpp>
#include "nibbler.hpp"
#include <cores/Univers.hpp>

class SpriteSystem : public KINU::System {
public:
	SpriteSystem(Univers &univers);
	SpriteSystem() = delete;
	~SpriteSystem() = default;
	SpriteSystem &operator=(const SpriteSystem &) = delete;
	SpriteSystem(const SpriteSystem &) = delete;

	virtual void update();
	static eSprite spriteDirection(PositionComponent &actual, PositionComponent &follow);

private:
	Univers &univers_;
};


