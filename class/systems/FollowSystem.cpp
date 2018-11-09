

#include <component/FollowComponent.hpp>
#include "FollowSystem.hpp"
#include <KNU/World.hpp>
#include <component/PositionComponent.hpp>

FollowSystem::FollowSystem() {
	requireComponent<FollowComponent>();
	requireComponent<PositionComponent>();
}


void FollowSystem::update() {
	log_success("update");
	for (const auto &entity : getEntities()) {

		auto &followComponent = entity.getComponent<FollowComponent>();
		auto entityFollowed = getWorld().getEntityManager()
				.getEntityById(followComponent.idFollowed);
		log_info("In {%d} entityFollowed.id : [%d] idFollowed [%d]", entity.getId(), entityFollowed.getId(), followComponent.idFollowed);
		assert(entityFollowed.hasComponent<PositionComponent>());
		if (!followComponent.skip)
			followComponent.positionComponent = entityFollowed.getComponent<PositionComponent>();
		followComponent.skip = false;
	}
	for (const auto &entity : getEntities()) {
		entity.getComponent<PositionComponent>() =
				entity.getComponent<FollowComponent>().positionComponent;
	}
}
