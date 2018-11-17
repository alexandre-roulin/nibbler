#include "KINU/World.hpp"
#include "RenderSystem.hpp"
#include <component/SpriteComponent.hpp>
#include <component/FollowComponent.hpp>
#include <component/PreviousComponent.hpp>
#include <logger.h>

RenderSystem::RenderSystem() {
	requireComponent<PositionComponent>();
	requireComponent<SpriteComponent>();
}

int direction(eSprite to) {
	if ((to & eSprite::SOUTH) == eSprite::SOUTH)
		return (0);
	if ((to & eSprite::WEST) == eSprite::WEST)
		return (1);
	if ((to & eSprite::NORTH) == eSprite::NORTH)
		return (2);
	return (3);
}

void RenderSystem::debugSpriteSnake_(eSprite sprite) {
	eSprite from = (sprite & eSprite::MASK_FROM) >> eSprite::BITWISE_FROM;
	eSprite to = (sprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO;

	std::cout << "Entity : [";

	if ((sprite & eSprite::MASK_BODY) == eSprite::HEAD)
		std::cout << "HEAD";
	if ((sprite & eSprite::MASK_BODY) == eSprite::BODY)
		std::cout << "BODY";
	if ((sprite & eSprite::MASK_BODY) == eSprite::TAIL)
		std::cout << "TAIL";

	std::cout << "] FROM [";
	if ((from & eSprite::NORTH) == eSprite::NORTH)
		std::cout << "NORTH";
	if ((from & eSprite::SOUTH) == eSprite::SOUTH)
		std::cout << "SOUTH";
	if ((from & eSprite::WEST) == eSprite::WEST)
		std::cout << "WEST";
	if ((from & eSprite::EAST) == eSprite::EAST)
		std::cout << "EAST";
	std::cout << "]TO [";
	if ((to & eSprite::NORTH) == eSprite::NORTH)
		std::cout << "NORTH";
	if ((to & eSprite::SOUTH) == eSprite::SOUTH)
		std::cout << "SOUTH";
	if ((to & eSprite::WEST) == eSprite::WEST)
		std::cout << "WEST";
	if ((to & eSprite::EAST) == eSprite::EAST)
		std::cout << "EAST";
	std::bitset<32> c(static_cast<int>(sprite));

	std::cout << "::  : [" << c << "]" << std::endl;
}


int RenderSystem::getSpriteSnake_(eSprite sprite) {

	eSprite from = (sprite & eSprite::MASK_FROM) >> eSprite::BITWISE_FROM;
	eSprite to = (sprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO;

	if ((sprite & eSprite::FOOD) == eSprite::FOOD)
		return (SPRITE_FOOD);
	if ((sprite & eSprite::MASK_BODY) == eSprite::HEAD)
		return (SIZE_LINE_TILESET * static_cast<int>(sprite & eSprite::MASK_COLOR) + 5 + direction(to));
	if ((sprite & eSprite::MASK_BODY) == eSprite::BODY) {

		if (from == to && (from == eSprite::NORTH || from == eSprite::SOUTH))
			return (SIZE_LINE_TILESET * static_cast<int>(sprite & eSprite::MASK_COLOR) + 14);
		else if (from == to && (from == eSprite::EAST || from == eSprite::WEST))
			return (SIZE_LINE_TILESET * static_cast<int>(sprite & eSprite::MASK_COLOR) + 13);

		else if (from == eSprite::NORTH && to == eSprite::EAST)
			return (SIZE_LINE_TILESET * static_cast<int>(sprite & eSprite::MASK_COLOR) + 1);
		else if (from == eSprite::NORTH && to == eSprite::WEST)
			return (SIZE_LINE_TILESET * static_cast<int>(sprite & eSprite::MASK_COLOR) + 2);

		else if (from == eSprite::SOUTH && to == eSprite::EAST)
			return (SIZE_LINE_TILESET * static_cast<int>(sprite & eSprite::MASK_COLOR) + 3);
		else if (from == eSprite::SOUTH && to == eSprite::WEST)
			return (SIZE_LINE_TILESET * static_cast<int>(sprite & eSprite::MASK_COLOR) + 4);

		else if (from == eSprite::WEST && to == eSprite::SOUTH)
			return (SIZE_LINE_TILESET * static_cast<int>(sprite & eSprite::MASK_COLOR) + 1);
		else if (from == eSprite::EAST && to == eSprite::SOUTH)
			return (SIZE_LINE_TILESET * static_cast<int>(sprite & eSprite::MASK_COLOR) + 2);

		else if (from == eSprite::WEST && to == eSprite::NORTH)
			return (SIZE_LINE_TILESET * static_cast<int>(sprite & eSprite::MASK_COLOR) + 3);
		else if (from == eSprite::EAST && to == eSprite::NORTH)
			return (SIZE_LINE_TILESET * static_cast<int>(sprite & eSprite::MASK_COLOR) + 4);

		else
			return (SIZE_LINE_TILESET);
	}
	if ((sprite & eSprite::MASK_BODY) == eSprite::TAIL)
		return (SIZE_LINE_TILESET * static_cast<int>(sprite & eSprite::MASK_COLOR) + 9 + direction(to));

	return (0);
}


void RenderSystem::update() {
	log_success("update");

	for (auto &entity : getEntities()) {
		if (entity.isAlive()) {
			auto& positionComponent = entity.getComponent<PositionComponent>();
			getWorld().grid(positionComponent.x, positionComponent.y) = RenderSystem::getSpriteSnake_(entity.getComponent<SpriteComponent>().sprite);
		}
	}
}



