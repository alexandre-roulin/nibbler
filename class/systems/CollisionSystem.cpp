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
			std::for_each(entities_.begin(), entities_.end(),
						  [&entity, &world](
								  KNU::Entity &entity_) {
							  auto &snakePositionComponent = entity.getComponent<PositionComponent>();
							  auto &positionComponent = entity_.getComponent<PositionComponent>();
							  if (snakePositionComponent == positionComponent && entity != entity_) {
								  auto &collisionComponent = entity_.getComponent<CollisionComponent>();
								  eSnakePart esp = Factory::isSnakePart(entity_.getTag());
								  log_warn("Collision between [%d] [%d]", entity_.getId(), entity.getId());
								  if (collisionComponent.isWall ||
									  esp != OTHER) {
								  	log_warn("Wall || SnakePart != HEAD");
									  entity.killGroup();
								  }
								  if (esp == HEAD) {
									  log_warn("Head");
									  entity.killGroup();
								  }
								  if (entity_.getGroup() == "food") {
									  log_warn("Food");
									  std::string s = Factory::factory_name(TAIL, entity.getTag());
									  std::cout << "food" << s << std::endl;
									  entity_.kill();
									  world.getEventManager()
											  .emitEvent<FoodEvent>(
													  s, entity.getGroup());
								  }
							  }
						  });
		}
	}
}


