#include "CollisionSystem.hpp"
#include <factory/Factory.hpp>
#include <component/CollisionComponent.hpp>
#include <component/PositionComponent.hpp>
#include <events/FoodEvent.hpp>
#include <logger.h>

CollisionSystem::CollisionSystem() {
	requireComponent<CollisionComponent>();
	requireComponent<PositionComponent>();
}


void CollisionSystem::update() {
	std::vector<KINU::Entity> entities_ = getEntities();
	auto &world = getWorld();
	for (auto &entity : getEntities()) {
		if (Factory::isSnakePart(entity.getTag()) == HEAD) {
			std::for_each(entities_.begin(), entities_.end(),
						  [&entity, &world](
								  KINU::Entity &entity_) {
							  auto &snakePositionComponent = entity.getComponent<PositionComponent>();
							  auto &positionComponent = entity_.getComponent<PositionComponent>();
							  if (snakePositionComponent == positionComponent &&
								  entity != entity_) {
								  auto &collisionComponent = entity_.getComponent<CollisionComponent>();
								  eSnakePart esp = Factory::isSnakePart(
										  entity_.getTag());
								  log_warn("Collision between [%d] [%d]",
										   entity_.getIndex(), entity.getIndex());
								  if (collisionComponent.isWall ||
									  esp != OTHER) {
									  log_warn("Wall || SnakePart != HEAD");
									  entity.killGroup();
								  } else if (esp == HEAD) {
									  log_warn("Head");
									  entity.killGroup();
								  } else { //if (entity_.getGroup() == "food")
									  log_warn("Food");
									  entity_.kill();
									  world.getEventManager()
											  .emitEvent<FoodEvent>(
													  Factory::factory_name(
															  TAIL,
															  entity.getTag()),
													  entity.getGroup());
								  }
							  }
						  });
		}
	}
}


