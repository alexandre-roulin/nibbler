

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
		if (followComponent.skip) {
			followComponent.skip = false;
			log_info("Skip [%d] -> from pos y : %d, x : %d SkipAfter [%d]",item.getId(), positionComponent.y, positionComponent.x, item.getComponent<FollowComponent>().skip);

		}
		else
			positionComponent = positionComponentFollowed;
	}
}
