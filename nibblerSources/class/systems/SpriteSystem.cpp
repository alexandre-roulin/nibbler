#include "KINU/World.hpp"
#include "SpriteSystem.hpp"
#include <component/SpriteComponent.hpp>
#include <component/FollowComponent.hpp>
#include <component/MotionComponent.hpp>
#include <cores/Univers.hpp>

SpriteSystem::SpriteSystem(Univers &univers) : univers_(univers) {
	requireComponent<PositionComponent>();
	requireComponent<SpriteComponent>();
}

eSprite SpriteSystem::spriteDirection(PositionComponent &actual,
									  PositionComponent &follow) {
	PositionComponent res;

	res.x = actual.x - follow.x;
	res.y = actual.y - follow.y;

	//Borderless : Position of Follow can be opposite of Actual
	if ((res.y && std::abs(res.y) != 1) || (res.x && std::abs(res.x) != 1)) {
		res.x = res.x * -1;
		res.y = res.y * -1;
	}

	if (res.y < 0)
		return (eSprite::kSouth);
	else if (res.x > 0)
		return (eSprite::kWest);
	else if (res.y > 0)
		return (eSprite::kNorth);
	return (eSprite::kEast);
}

void SpriteSystem::update() {

	FollowComponent *followComponent = nullptr;
	KINU::Entity entityFollowed;
	PositionComponent positionComponentFollowed;

	for (auto &entity : getEntities()) {
		auto &spriteComponent = entity.getComponent<SpriteComponent>();

		spriteComponent.sprite = spriteComponent.sprite & (0xFFFFFFFF ^
														   (eSprite::kMaskDirection |
															eSprite::kMaskFrom |
															eSprite::kMaskTo));
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

		SnakeClient::boost_shared_ptr ptr(univers_.getSnakeClient().lock());

		if ((spriteComponent.sprite & eSprite::kHead) == eSprite::kHead) {
			if (entity.getComponent<MotionComponent>().direction == kNorth)
				spriteComponent.sprite |= eSprite::kToNorth;
			else if (entity.getComponent<MotionComponent>().direction ==
					 kSouth)
				spriteComponent.sprite |= eSprite::kToSouth;
			else if (entity.getComponent<MotionComponent>().direction ==
					 kEast)
				spriteComponent.sprite |= eSprite::kToEast;
			else if (entity.getComponent<MotionComponent>().direction ==
					 kWest)
				spriteComponent.sprite |= eSprite::kToWest;
			if (entity.hasGroupId() && ptr && ptr->getId_() == entity.getGroupIdByEntity()) {
				spriteComponent.sprite |= eSprite::kYourSnake;
			}
		} else {
			spriteComponent.sprite |=
					(SpriteSystem::spriteDirection(positionComponent,
												   positionComponentFollowed)
							<< eSprite::kBitwiseTo);
		}

		if (followComponent && getWorld().getEntitiesManager().hasEntityById(followComponent->idFollowed)) {
			entityFollowed.getComponent<SpriteComponent>().sprite |=
					(SpriteSystem::spriteDirection(positionComponent,
												   positionComponentFollowed)
							<< eSprite::kBitwiseFrom);
		}
	}
}