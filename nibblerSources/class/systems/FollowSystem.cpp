

#include "FollowSystem.hpp"
#include <KINU/World.hpp>
#include <component/FollowComponent.hpp>
#include <component/PositionComponent.hpp>
#include <logger.h>

FollowSystem::FollowSystem() {
	requireComponent<FollowComponent>();
	requireComponent<PositionComponent>();
}


void FollowSystem::update() {
//	log_info("%s", __PRETTY_FUNCTION__);
	for (const auto &entity : getEntities()) {
		auto &followComponent = entity.getComponent<FollowComponent>();
		if (getWorld().getEntitiesManager().hasEntityById(followComponent.idFollowed)) {
			auto entityFollowed = getWorld().getEntitiesManager().getEntityById(followComponent.idFollowed);
			if (!followComponent.skip)
				followComponent.positionComponent = entityFollowed.getComponent<PositionComponent>();
			followComponent.skip = false;
		}

	}
	for (const auto &entity : getEntities()) {
		auto &positionComponent = entity.getComponent<PositionComponent>();
		positionComponent = entity.getComponent<FollowComponent>().positionComponent;
	}
}
