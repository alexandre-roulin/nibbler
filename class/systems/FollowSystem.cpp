

#include <component/FollowComponent.hpp>
#include "FollowSystem.hpp"
#include <KNU/World.hpp>
#include <component/PositionComponent.hpp>

FollowSystem::FollowSystem() {
	requireComponent<FollowComponent>();
	requireComponent<PositionComponent>();
}


void FollowSystem::update() {
	for (const auto &entity : getEntities()) {
		auto &followComponent = entity.getComponent<FollowComponent>();
		auto entityFollowed = getWorld().getEntityManager()
				.getEntityById(followComponent.idFollowed);
		assert(entityFollowed.hasComponent<PositionComponent>());
		followComponent.positionComponent = entityFollowed.getComponent<PositionComponent>();
	}
	for (const auto &entity : getEntities()) {
		entity.getComponent<PositionComponent>() =
				entity.getComponent<FollowComponent>().positionComponent;
	}
}
