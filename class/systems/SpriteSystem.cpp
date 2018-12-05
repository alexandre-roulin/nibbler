#include "KINU/World.hpp"
#include "SpriteSystem.hpp"
#include <component/SpriteComponent.hpp>
#include <component/FollowComponent.hpp>
#include <component/MotionComponent.hpp>
#include <logger.h>

SpriteSystem::SpriteSystem() {
	requireComponent<PositionComponent>();
	requireComponent<SpriteComponent>();
}

eSprite SpriteSystem::spriteDirection(PositionComponent &actual,
									  PositionComponent &follow) {
	PositionComponent res;

	res.x = actual.x - follow.x;
	res.y = actual.y - follow.y;
	if (res.y < 0)
		return (eSprite::SOUTH);
	else if (res.x > 0)
		return (eSprite::WEST);
	else if (res.y > 0)
		return (eSprite::NORTH);
	return (eSprite::EAST);
}

void SpriteSystem::update() {

	FollowComponent *followComponent = nullptr;
	KINU::Entity entityFollowed;
	PositionComponent positionComponentFollowed;

	for (auto &entity : getEntities()) {
			auto &positionComponent = entity.getComponent<PositionComponent>();
			auto &spriteComponent = entity.getComponent<SpriteComponent>();

			spriteComponent.sprite = spriteComponent.sprite & (0xFFFFFFFF ^
															   (eSprite::MASK_DIRECTION |
																eSprite::MASK_FROM |
																eSprite::MASK_TO));
	}

	for (auto &entity : getEntities()) {
		followComponent = nullptr;
			auto &positionComponent = entity.getComponent<PositionComponent>();
			auto &spriteComponent = entity.getComponent<SpriteComponent>();

			if (entity.hasComponent<FollowComponent>()) {
				followComponent = &entity.getComponent<FollowComponent>();
				entityFollowed = getWorld().getEntitiesManager().getEntityById(
						followComponent->idFollowed);
				positionComponentFollowed = entityFollowed.getComponent<PositionComponent>();
			}

			if ((spriteComponent.sprite & eSprite::HEAD) == eSprite::HEAD) {
				if (entity.getComponent<MotionComponent>().direction == NORTH)
					spriteComponent.sprite |= eSprite::TO_NORTH;
				else if (entity.getComponent<MotionComponent>().direction ==
						 SOUTH)
					spriteComponent.sprite |= eSprite::TO_SOUTH;
				else if (entity.getComponent<MotionComponent>().direction ==
						 EAST)
					spriteComponent.sprite |= eSprite::TO_EAST;
				else if (entity.getComponent<MotionComponent>().direction ==
						 WEST)
					spriteComponent.sprite |= eSprite::TO_WEST;
			} else {
				spriteComponent.sprite |=
						(SpriteSystem::spriteDirection(positionComponent,
													   positionComponentFollowed)
								<< eSprite::BITWISE_TO);
			}

			if (followComponent) {
				entityFollowed.getComponent<SpriteComponent>().sprite |=
						(SpriteSystem::spriteDirection(positionComponent,
													   positionComponentFollowed)
								<< eSprite::BITWISE_FROM);
			}
	}
}