#include "KINU/World.hpp"
#include "SpriteSystem.hpp"
#include <component/SpriteComponent.hpp>
#include <component/FollowComponent.hpp>
#include <component/MotionComponent.hpp>

SpriteSystem::SpriteSystem(Univers &univers) : univers_(univers) {
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
		auto &spriteComponent = entity.getComponent<SpriteComponent>();

		spriteComponent.sprite = spriteComponent.sprite & (0xFFFFFFFF ^
														   (eSprite::MASK_DIRECTION |
															eSprite::MASK_FROM |
															eSprite::MASK_TO));
	}

	for (auto entity : getEntities()) {
		followComponent = nullptr;
		auto &positionComponent = entity.getComponent<PositionComponent>();
		auto &spriteComponent = entity.getComponent<SpriteComponent>();

		if (entity.hasComponent<FollowComponent>()) {
			followComponent = &entity.getComponent<FollowComponent>();
			if (getWorld().getEntitiesManager().hasEntityById(followComponent->idFollowed)) {
				entityFollowed = getWorld().getEntitiesManager().getEntityById(
						followComponent->idFollowed);
				positionComponentFollowed = entityFollowed.getComponent<PositionComponent>();
			}
		}

		if ((spriteComponent.sprite & eSprite::HEAD) == eSprite::HEAD) {
			if (entity.getComponent<MotionComponent>().direction == kNorth)
				spriteComponent.sprite |= eSprite::TO_NORTH;
			else if (entity.getComponent<MotionComponent>().direction ==
					 kSouth)
				spriteComponent.sprite |= eSprite::TO_SOUTH;
			else if (entity.getComponent<MotionComponent>().direction ==
					 kEast)
				spriteComponent.sprite |= eSprite::TO_EAST;
			else if (entity.getComponent<MotionComponent>().direction ==
					 kWest)
				spriteComponent.sprite |= eSprite::TO_WEST;
			if (entity.hasGroupId() && univers_.getSnakeClient()->getId_() == entity.getGroupIdByEntity()) {
				spriteComponent.sprite |= eSprite::YOUR_SNAKE;
			}
		} else {
			spriteComponent.sprite |=
					(SpriteSystem::spriteDirection(positionComponent,
												   positionComponentFollowed)
							<< eSprite::BITWISE_TO);
		}

		if (followComponent && getWorld().getEntitiesManager().hasEntityById(followComponent->idFollowed)) {
			entityFollowed.getComponent<SpriteComponent>().sprite |=
					(SpriteSystem::spriteDirection(positionComponent,
												   positionComponentFollowed)
							<< eSprite::BITWISE_FROM);
		}
	}
}