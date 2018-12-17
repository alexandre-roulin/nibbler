#include "CollisionSystem.hpp"
#include <factory/Factory.hpp>
#include <events/FoodEat.hpp>
#include <network/ClientTCP.hpp>
#include <logger.h>
#include <Grid.tpp>
#include <component/CollisionComponent.hpp>
#include <KINU/Entity.hpp>
#include <KINU/World.hpp>

CollisionSystem::CollisionSystem() {
	requireComponent<CollisionComponent>();
	requireComponent<PositionComponent>();
}


void CollisionSystem::checkCollision(
		KINU::Entity entityHead, KINU::Entity entityCheck) {

	auto &snakePositionComponent = entityHead.getComponent<PositionComponent>();
	auto &positionComponent = entityCheck.getComponent<PositionComponent>();

	if (snakePositionComponent == positionComponent &&
		entityHead != entityCheck) {
		auto &collisionComponent = entityCheck.getComponent<CollisionComponent>();
		KINU::TagId tagId = entityCheck.getGroupIdByEntity();

		if (tagId == eTag::FOOD_TAG) {
			log_info("FOOD_TAG::FoodCollision");

			getWorld().getUnivers().playNoise(eSound::FOOD);

			//delete
			log_warn("Head collision x[%d] y[%d] id[%d]",snakePositionComponent.x, snakePositionComponent.y, entityHead.getGroupIdByEntity());
			//
			entityCheck.kill();
			getWorld().getEventsManager().emitEvent<FoodEat>(entityHead.getGroupIdByEntity());

			if (getWorld().getUnivers().getClientTCP_().getId() == entityHead.getGroupIdByEntity() ||
				getWorld().getUnivers().isIASnake(entityHead.getGroupIdByEntity())) {

				ClientTCP::FoodInfo foodInfo;
				foodInfo.positionComponent = PositionComponent(
						getWorld().grid.getRandomSlot(FREE_SLOT));
				foodInfo.fromSnake = false;

				getWorld().getUnivers().getClientTCP_().write_socket(
						ClientTCP::add_prefix(FOOD, &foodInfo));
			}
		} else if (tagId == eTag::FOOD_TAG_FROM_SNAKE) {
			log_info("FOOD_TAG_FROM_SNAKE::FoodCollision");
			getWorld().getUnivers().playNoise(eSound::FOOD);
			entityCheck.kill();
			getWorld().getEventsManager().emitEvent<FoodEat>(entityHead.getGroupIdByEntity());
		} else if (tagId == WALL_TAG) {
			log_info("WALL_TAG::WallCollision");
			if (entityHead.getGroupIdByEntity() == getWorld().getUnivers().getClientTCP_().getId() ||
					getWorld().getUnivers().isIASnake(entityHead.getGroupIdByEntity()))
				getWorld().getUnivers().getClientTCP_().killSnake(entityHead.getGroupIdByEntity());
			entityHead.killGroup();

		} else if (entityCheck.getGroupIdByEntity() == entityHead.getGroupIdByEntity()) {
			log_info("HIMSELF::CollisionOnHimself");
			if (entityHead.getGroupIdByEntity() == getWorld().getUnivers().getClientTCP_().getId() ||
					getWorld().getUnivers().isIASnake(entityHead.getGroupIdByEntity()))
				getWorld().getUnivers().getClientTCP_().killSnake(entityHead.getGroupIdByEntity());
			entityHead.killGroup();
		} else {
			log_info("SNAKE::CollisionSnake");

			auto appleSnake = getWorld().getEntitiesManager().getEntitiesByGroupId(entityHead.getGroupIdByEntity());
			for (auto snake : appleSnake) {
				ClientTCP::FoodInfo foodInfo;
				foodInfo.positionComponent = snake.getComponent<PositionComponent>();
				foodInfo.fromSnake = true;
				if (snake.hasComponent<PositionComponent>())
					getWorld().getUnivers().getClientTCP_().write_socket(
							ClientTCP::add_prefix(FOOD, &foodInfo));
			}
			if (entityHead.getGroupIdByEntity() == getWorld().getUnivers().getClientTCP_().getId() ||
					getWorld().getUnivers().isIASnake(entityHead.getGroupIdByEntity()))
				getWorld().getUnivers().getClientTCP_().killSnake(entityHead.getGroupIdByEntity());

			entityHead.killGroup();
		}

	}
}

void CollisionSystem::update() {
	std::vector<KINU::Entity> entities_ = getEntities();
	auto &world = getWorld();
	for (auto entity : getEntities()) {
		if (entity.hasTagId() &&
			entity.getTagId() - entity.getGroupIdByEntity() == eTag::HEAD_TAG) {

			for (auto entityCheck : entities_) {
				checkCollision(entity, entityCheck);
			}
		}
	}
}


