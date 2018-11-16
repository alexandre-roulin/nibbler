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

void RenderSystem::update() {
	log_success("update");

	PositionComponent followPosition, positionComponentFollowed, positionComponentPrevious;

	for (auto &entity : getEntities()) {
		if (entity.isAlive()) {
			auto& positionComponent = entity.getComponent<PositionComponent>();
			auto& spriteComponent = entity.getComponent<SpriteComponent>();

			if (entity.hasComponent<FollowComponent>()) {
				auto *followComponent = &entity.getComponent<FollowComponent>();
				auto entityFollowed = getWorld().getEntityManager().getEntity(followComponent->idFollowed);
				positionComponentFollowed = entityFollowed.getComponent<PositionComponent>();
			}
			if (entity.hasComponent<PreviousComponent>()) {
				auto *previousComponent = &entity.getComponent<PreviousComponent>();
				auto entityPrevious = getWorld().getEntityManager().getEntity(previousComponent->idPrevious);
				positionComponentPrevious = entityPrevious.getComponent<PositionComponent>();
			}

			getWorld().grid(positionComponent.x, positionComponent.y) = RenderSystem::getSpriteSnake_(spriteComponent.sprite, positionComponentPrevious, positionComponent, positionComponentFollowed);
		}
	}
}

int direction(PositionComponent &actual, PositionComponent &follow) {
	PositionComponent res;

	res.x = actual.x - follow.x;
	res.y = actual.y - follow.y;
	if (res.y < 0)
		return (0);
	else if (res.x > 0)
		return (1);
	else if (res.y > 0)
		return (2);
	return (3);
}
int edirection(PositionComponent &actual, PositionComponent &follow) {
	PositionComponent res;

	res.x = actual.x - follow.x;
	res.y = actual.y - follow.y;
	if (res.y < 0)
		return (SOUTH);
	else if (res.x > 0)
		return (WEST);
	else if (res.y > 0)
		return (NORTH);
	return (EAST);
}

int RenderSystem::getSpriteSnake_(eSprite sprite, PositionComponent &previous, PositionComponent &actual, PositionComponent &follow) {
	int isprite = static_cast<int>(sprite);

	if ((isprite & eSprite::FOOD) == eSprite::FOOD)
		return (SPRITE_FOOD);
	if ((isprite & eSprite::MASK_BODY) == eSprite::HEAD)
		return (SIZE_LINE_TILESET * static_cast<int>(isprite & eSprite::MASK_COLOR) + 5);
	if ((isprite & eSprite::MASK_BODY) == eSprite::BODY) {

		if (previous.x == actual.x && actual.x == follow.x)
			return (SIZE_LINE_TILESET * static_cast<int>(isprite & eSprite::MASK_COLOR) + 14);
		else if (previous.y == actual.y && actual.y == follow.y)
			return (SIZE_LINE_TILESET * static_cast<int>(isprite & eSprite::MASK_COLOR) + 13);

		else if (edirection(previous, actual) == NORTH && edirection(actual, follow) == EAST)
			return (SIZE_LINE_TILESET * static_cast<int>(isprite & eSprite::MASK_COLOR) + 1);
		else if (edirection(previous, actual) == NORTH && edirection(actual, follow) == WEST)
			return (SIZE_LINE_TILESET * static_cast<int>(isprite & eSprite::MASK_COLOR) + 2);

		else if (edirection(previous, actual) == SOUTH && edirection(actual, follow) == EAST)
			return (SIZE_LINE_TILESET * static_cast<int>(isprite & eSprite::MASK_COLOR) + 3);
		else if (edirection(previous, actual) == SOUTH && edirection(actual, follow) == WEST)
			return (SIZE_LINE_TILESET * static_cast<int>(isprite & eSprite::MASK_COLOR) + 4);

		else if (edirection(previous, actual) == WEST && edirection(actual, follow) == SOUTH)
			return (SIZE_LINE_TILESET * static_cast<int>(isprite & eSprite::MASK_COLOR) + 1);
		else if (edirection(previous, actual) == EAST && edirection(actual, follow) == SOUTH)
			return (SIZE_LINE_TILESET * static_cast<int>(isprite & eSprite::MASK_COLOR) + 2);

		else if (edirection(previous, actual) == WEST && edirection(actual, follow) == NORTH)
			return (SIZE_LINE_TILESET * static_cast<int>(isprite & eSprite::MASK_COLOR) + 3);
		else if (edirection(previous, actual) == EAST && edirection(actual, follow) == NORTH)
			return (SIZE_LINE_TILESET * static_cast<int>(isprite & eSprite::MASK_COLOR) + 4);

		else
			return (SIZE_LINE_TILESET);
	}
	if ((isprite & eSprite::MASK_BODY) == eSprite::TAIL)
		return (SIZE_LINE_TILESET * static_cast<int>(isprite & eSprite::MASK_COLOR) + 9 + direction(actual, follow));

	return (0);
}


