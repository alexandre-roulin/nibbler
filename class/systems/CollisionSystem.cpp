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
									std::cout << esp << std::endl;
								  if (collisionComponent.isWall ||
									  esp != OTHER) {
									  std::cout << "wall [" << collisionComponent.isWall << "]" << std::endl;
									  entity.killGroup();
								  }
								  if (esp == HEAD) {
									  std::cout << "HEAD" << std::endl;
										entity.killGroup();
								  }
								  if (entity_.getTag() == "food") {
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


