#include <component/PositionComponent.hpp>
#include <component/CollisionComponent.hpp>
#include "CollisionSystem.hpp"
#include "KNU/World.hpp"
#include <events/FoodEvent.hpp>

CollisionSystem::CollisionSystem() {
	requireComponent<CollisionComponent>();
	requireComponent<PositionComponent>();
}


void CollisionSystem::update() {
	std::vector<KNU::Entity> v = getEntities();
	auto &world = getWorld();
	for (auto &entity : getEntities()) {
		auto isCollectable = entity.getComponent<CollisionComponent>()
				.isCollectable;
		if (entity != world.getEntityManager().getEntityByTag(TAG_HEAD_SNAKE))
			std::for_each(v.begin(), v.end(),
						  [&world, isCollectable](KNU::Entity &entity) {
							  auto snake_head = world.getEntityManager()
									  .getEntityByTag(TAG_HEAD_SNAKE);
							  if (entity != snake_head &&
								  (snake_head.getComponent<PositionComponent>() ==
								   entity.getComponent<PositionComponent>())) {
								  if (isCollectable) {
									  entity.kill();
									  world.getEventManager()
											  .emitEvent<FoodEvent>(false);
								  } else {
									  snake_head.kill();
								  }
							  }
						  });
	}

}


