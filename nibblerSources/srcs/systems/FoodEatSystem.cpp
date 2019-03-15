#include <component/PositionComponent.hpp>
#include <component/FollowComponent.hpp>
#include <component/CollisionComponent.hpp>
#include <nibbler.hpp>
#include <component/SpriteComponent.hpp>
#include "FoodEatSystem.hpp"
#include <KINU/World.hpp>
#include <factory/Factory.hpp>
#include <events/FoodEat.hpp>

void FoodEatSystem::update() {
	auto events = getWorld().getEventsManager().getEvents<FoodEat>();
	for (auto &event : events) {
		if (getWorld().getEntitiesManager().hasEntityByTagId(event.id_ + eTag::kTailTag)) {

			auto entityTail = getWorld().getEntitiesManager()
					.getEntityByTagId(event.id_ + eTag::kTailTag);
			auto newEntity = getWorld().createEntity();
			auto followComponent = entityTail.getComponent<FollowComponent>();
			newEntity.addComponent<PositionComponent>(
					followComponent.positionComponent);
			newEntity.addComponent<FollowComponent>(
					followComponent.idFollowed, false);
			newEntity.addComponent<CollisionComponent>();

			assert(getWorld().getEntitiesManager().hasEntityById(entityTail.getComponent<FollowComponent>().idFollowed));
			auto entity = getWorld().getEntitiesManager().getEntityById(
					entityTail.getComponent<FollowComponent>().idFollowed);
			newEntity.addComponent<SpriteComponent>(entity.getComponent<SpriteComponent>());

			auto &followComponentRef = entityTail.getComponent<FollowComponent>();
			followComponentRef.idFollowed = newEntity.getId();
			followComponentRef.skip = true;

			newEntity.groupEntityByGroupId(entityTail.getGroupIdByEntity());

		}
	}

}
