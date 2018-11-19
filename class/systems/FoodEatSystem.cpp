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
	auto events = getWorld().getEventManager().getEvents<FoodEat>();

	for (auto &event : events) {
		auto entityTail = getWorld().getEntityManager()
				.getEntityByTag(Factory::factory_name(TAIL, event.id_));
		auto newEntity = getWorld().createEntity();

// Position == entityTail.positionComponent
		newEntity.addComponent<PositionComponent>(
				entityTail.getComponent<PositionComponent>());

//FollowComponent.id == entityTail.idFollowed
		newEntity.addComponent<FollowComponent>(
				entityTail.getComponent<FollowComponent>().idFollowed, false);
		newEntity.addComponent<CollisionComponent>();


// NEW : Make TO_direction
		newEntity.addComponent<SpriteComponent>(
				getWorld().getEntityManager().getEntity(
						entityTail.getComponent<FollowComponent>().idFollowed).getComponent<SpriteComponent>());

		auto &followComponent = entityTail.getComponent<FollowComponent>();
		followComponent.idFollowed = newEntity.getIndex();
		followComponent.skip = true;

		newEntity.tag(Factory::factory_name(BODY, newEntity.getIndex()));
		newEntity.group(entityTail.getGroup());

	}

}
