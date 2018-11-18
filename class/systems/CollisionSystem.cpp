#include "CollisionSystem.hpp"
#include <factory/Factory.hpp>
#include <events/FoodEat.hpp>
#include <events/FoodCreation.hpp>
#include <network/ClientTCP.hpp>

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
		log_info("[%d][%d][%s]", entityHead.getIndex(), entityCheck.getIndex(),
				 group.c_str());
		if (group == FOOD_TAG) {
			log_info("FoodCollision");
			entityCheck.kill();
			getWorld().getEventManager().emitEvent<FoodEat>(
					Factory::getIdFromTag(entityHead.getTag()));
			if (getWorld().getUnivers().getClientTCP_().getId() ==
				std::stoi(entityHead.getTag())) {
				auto positionFood = PositionComponent(rand() % (30 - 2) + 1, rand() % (30 - 2) + 1);
				getWorld().getUnivers().getClientTCP_().write_socket(ClientTCP::add_prefix(FOOD, &positionFood));
			}
		} else if (group == WALL_TAG) {
			log_info("WallCollision");
			entityHead.killGroup();
		} else if (entityCheck.getGroup() == entityHead.getGroup()) {
			log_info("Body||TailCollision");
			entityHead.killGroup();
		} else {
			log_info("Crash an other Snake");
			auto winSnake = getWorld().getEntityManager().getEntityGroup(entityHead.getGroup()).size();
			for (int index = 0; index < winSnake; ++index) {
				getWorld().getEventManager().emitEvent<FoodEat>(
						Factory::getIdFromTag(entityHead.getTag()));
			}
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


