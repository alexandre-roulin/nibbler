

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
	log_success("update");
	for (const auto &entity : getEntities()) {

		auto &followComponent = entity.getComponent<FollowComponent>();
		auto entityFollowed = getWorld().getEntityManager().getEntity(followComponent.idFollowed);
		if (!followComponent.skip)
			followComponent.positionComponent = entityFollowed.getComponent<PositionComponent>();
		followComponent.skip = false;
	}
	for (const auto &entity : getEntities()) {

		auto &positionComponent = entity.getComponent<PositionComponent>();
		positionComponent = entity.getComponent<FollowComponent>().positionComponent;

		log_info("Entity [%d] x [%d] y [%d] fl [%d]", entity.getIndex(),
				 positionComponent.x, positionComponent.y ,entity.getComponent<FollowComponent>().idFollowed);

	}
}
