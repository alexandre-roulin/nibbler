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
	std::cout << "Size : " << getEntities().size() << std::endl;

	for (auto &entity : getEntities()) {
		if (Factory::isSnakePart(entity.getTag()) != OTHER)
			std::cout << "isSnakePart.() "
					  << Factory::isSnakePart(entity.getTag())
					  << "Head : " << HEAD << " Other: " << OTHER << std::endl;
		if (Factory::isSnakePart(entity.getTag()) == HEAD) {
			std::for_each(entities_.begin(), entities_.end(),
						  [&entity, &world](
								  KNU::Entity &entity_) {
							  auto &snakePositionComponent = entity.getComponent<PositionComponent>();
							  auto &positionComponent = entity_.getComponent<PositionComponent>();
//							  std::cout << "Snake [" << entity.getId() << "]"
//										<< snakePositionComponent << std::endl;
//							  std::cout << "Entity [" << entity_.getId() << "]"
//										<< positionComponent << std::endl;
//							  std::cout << std::endl;
							  if (snakePositionComponent == positionComponent && entity != entity_) {
								  auto &collisionComponent = entity_.getComponent<CollisionComponent>();
								  eSnakePart esp = Factory::isSnakePart(entity_.getTag());
									std::cout << esp << std::endl;
								  if (collisionComponent.isWall ||
									  esp != OTHER) {
									  std::cout << "wall [" << collisionComponent.isWall << "]" << std::endl;
//									  entity.kill();
								  }
								  if (esp == HEAD) {
									  std::cout << "HEAD" << std::endl;
//									  entity_.kill();

								  }
								  if (entity_.getTag() == "food") {
									  std::cout << "food" << std::endl;

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


