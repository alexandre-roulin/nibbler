#include <component/CollectComponent.hpp>
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

	if (!events.empty()) {
		auto entityTail = getWorld().getEntityManager()
				.getEntityByTag(TAG_TAIL_SNAKE);
		auto newEntity = getWorld().createEntity();
		auto &followComponent = entityTail.getComponent<FollowComponent>(); //SNAKE TAIL WITH ID FOLLOW
		auto &positionComponent = entityTail.getComponent<PositionComponent>(); //SNAKE TAIL WITH ID FOLLOW
		newEntity.addComponent<FollowComponent>(followComponent._idFollowed);
		newEntity.addComponent<PositionComponent>(positionComponent);
		newEntity.group(GROUP_SNAKE);
		followComponent._idFollowed = newEntity.getId();
		followComponent.skip = true;
	}
}
