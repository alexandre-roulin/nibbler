#include "CollisionSystem.hpp"
#include <factory/Factory.hpp>
#include <events/FoodEat.hpp>

CollisionSystem::CollisionSystem() {
	requireComponent<CollisionComponent>();
	requireComponent<PositionComponent>();
}


void CollisionSystem::checkCollision(
KINU::Entity &entityHead, KINU::Entity &entityCheck) {

	auto &snakePositionComponent = entityHead.getComponent<PositionComponent>();
	auto &positionComponent = entityCheck.getComponent<PositionComponent>();

	if (snakePositionComponent == positionComponent &&
		entityHead != entityCheck) {
		auto &collisionComponent = entityCheck.getComponent<CollisionComponent>();
		std::string group = entityCheck.getGroup();
		log_info("[%d][%d][%s]", entityHead.getIndex(), entityCheck.getIndex(), group.c_str());
		if (group == FOOD_TAG) {
			log_info("FoodCollision");
			entityCheck.kill();
			getWorld().getEventManager().emitEvent<FoodEat>(Factory::getIdFromTag(entityHead.getTag()));
			getWorld().getEventManager().emitEvent<FoodEat>(Factory::getIdFromTag(entityHead.getTag()));
			getWorld().getEventManager().emitEvent<FoodEat>(Factory::getIdFromTag(entityHead.getTag()));
		} else if (group == WALL_TAG) {
			log_info("WallCollision");
			entityHead.killGroup();
		} else if (!entityCheck.getTag().empty()){
			log_info("HeadCollision");
			entityHead.killGroup();
			entityCheck.killGroup();
		} else if (entityCheck.getGroup() != entityHead.getGroup())  {
			log_info("Body||TailCollision");
			entityHead.killGroup();
		}
	}
}

void CollisionSystem::update() {
	log_success("update");
	std::vector<KINU::Entity> entities_ = getEntities();
	auto &world = getWorld();
	for (auto &entity : getEntities()) {
		if (Factory::isSnakePart(entity.getTag()) == HEAD) {
			for (auto &entityCheck : entities_) {
				checkCollision(entity, entityCheck);
			}
		}
	}
}


