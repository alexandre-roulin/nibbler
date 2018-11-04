#include <component/PositionComponent.hpp>
#include "CollisionSystem.hpp"
#include <factory/Factory.hpp>
#include <events/FoodEvent.hpp>

CollisionSystem::CollisionSystem() {
	requireComponent<CollisionComponent>();
	requireComponent<PositionComponent>();
}


void CollisionSystem::update() {
	std::vector<KNU::Entity> entities_ = getEntities();
	auto &world = getWorld();

	for (auto &entity : getEntities()) {
		if (Factory::isSnakePart(entity.getTag()) == HEAD) {
			auto &snakePositionComponent = entity.getComponent<PositionComponent>();
			std::for_each(entities_.begin(), entities_.end(),
						  [&entity, snakePositionComponent, &world](
								  KNU::Entity &entity_) {
							  auto &positionComponent = entity_.getComponent<PositionComponent>();
							  if (snakePositionComponent == positionComponent) {
								  auto &collisionComponent = entity_.getComponent<CollisionComponent>();
								  eSnakePart esp = Factory::isSnakePart(
										  entity_.getTag());
								  if (collisionComponent.isWall ||
									  esp != OTHER) {
									  entity.kill();
								  }
								  if (esp == HEAD)
									  entity_.kill();
								  if (entity_.getTag() == "food")
									  world.getEventManager()
											  .emitEvent<FoodEvent>(
													  Factory::factory_name(
															  TAIL,
															  entity.getTag()),
													  entity.getGroup());
							  }
						  });
		}
	}
}


