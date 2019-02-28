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
//	log_info("%s", __PRETTY_FUNCTION__);
	auto events = getWorld().getEventsManager().getEvents<FoodEat>();
	for (auto &event : events) {
//		log_debug("Find Snake %d",getWorld().getEntitiesManager().hasEntityByTagId(event.id_ + eTag::kTailTag));
		if (getWorld().getEntitiesManager().hasEntityByTagId(event.id_ + eTag::kTailTag)) {

			auto entityTail = getWorld().getEntitiesManager()
					.getEntityByTagId(event.id_ + eTag::kTailTag);
			auto newEntity = getWorld().createEntity();

// Position == entityTail.positionComponent
			newEntity.addComponent<PositionComponent>(
					entityTail.getComponent<PositionComponent>());

//FollowComponent.id == entityTail.idFollowed
			newEntity.addComponent<FollowComponent>(
					entityTail.getComponent<FollowComponent>().idFollowed, false);
			newEntity.addComponent<CollisionComponent>();


// NEW : Make TO_direction
			assert(getWorld().getEntitiesManager().hasEntityById(entityTail.getComponent<FollowComponent>().idFollowed));
			auto entity = getWorld().getEntitiesManager().getEntityById(
					entityTail.getComponent<FollowComponent>().idFollowed);
			newEntity.addComponent<SpriteComponent>(entity.getComponent<SpriteComponent>());

			auto &followComponent = entityTail.getComponent<FollowComponent>();
			followComponent.idFollowed = newEntity.getId();
			followComponent.skip = true;

			newEntity.groupEntityByGroupId(entityTail.getGroupIdByEntity());

		}
	}

}
