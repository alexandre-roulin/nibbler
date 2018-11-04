#include <component/CollisionComponent.hpp>
#include "FoodSystem.hpp"
#include <KNU/World.hpp>
#include <events/FoodEvent.hpp>
#include <component/FollowComponent.hpp>

FoodSystem::FoodSystem() {
	requireComponent<FollowComponent>();
	requireComponent<PositionComponent>();
}

void FoodSystem::update() {
	auto events = getWorld().getEventManager().getEvents<FoodEvent>();

	for (auto &event : events) {
		auto entityTail = getWorld().getEntityManager().getEntityByTag(event.tag_tail);
		auto newEntity = getWorld().createEntity();

		auto &followComponent = entityTail.getComponent<FollowComponent>(); //SNAKE TAIL WITH ID FOLLOW
		auto &positionComponent = entityTail.getComponent<PositionComponent>(); //SNAKE TAIL WITH ID FOLLOW
		newEntity.addComponent<FollowComponent>(followComponent.idFollowed);
		newEntity.addComponent<PositionComponent>(positionComponent);
		newEntity.group(event.tag_group);
		followComponent.idFollowed = newEntity.getId();
		followComponent.skip = true;
	}
}
