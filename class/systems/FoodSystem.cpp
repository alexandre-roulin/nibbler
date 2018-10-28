#include <component/CollisionComponent.hpp>
#include "FoodSystem.hpp"
#include <KNU/World.hpp>
#include <events/FoodEvent.hpp>
#include <component/FollowComponent.hpp>
#include <component/PositionComponent.hpp>

FoodSystem::FoodSystem() {
	requireComponent<FollowComponent>();
	requireComponent<PositionComponent>();
}

void FoodSystem::update() {
	auto events = getWorld().getEventManager().getEvents<FoodEvent>();

	for (auto &event : events) {
		if (!event.consume) {
			log_success("Food Find Events size %d", events.size());
			auto entityTail = getWorld().getEntityManager()
					.getEntityByTag(TAG_TAIL_SNAKE);
			auto newEntity = getWorld().createEntity();
			auto &followComponent = entityTail.getComponent<FollowComponent>(); //SNAKE TAIL WITH ID FOLLOW
			auto &positionComponent = entityTail.getComponent<PositionComponent>(); //SNAKE TAIL WITH ID FOLLOW
			newEntity
			.addComponent<FollowComponent>(followComponent.idFollowed);
			newEntity.addComponent<PositionComponent>(positionComponent);
			newEntity.group(GROUP_SNAKE);
			followComponent.idFollowed = newEntity.getId();
			followComponent.skip = true;
			event.consume = true;
		}
	}
}
