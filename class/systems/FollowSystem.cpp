

#include <component/FollowComponent.hpp>
#include "FollowSystem.hpp"
#include <KNU/World.hpp>
#include <component/PositionComponent.hpp>

FollowSystem::FollowSystem() {
	requireComponent<FollowComponent>();
	requireComponent<PositionComponent>();
}


void FollowSystem::update() {
	for (const auto &item : getEntities()) {
		auto &followComponent = item.getComponent<FollowComponent>();
		auto &positionComponent = item.getComponent<PositionComponent>();

		auto positionComponentFollowed = getWorld().getEntityManager()
				.getEntityById(followComponent._idFollowed)
				.getComponent<PositionComponent>();
		positionComponent = positionComponentFollowed;
	}
}
