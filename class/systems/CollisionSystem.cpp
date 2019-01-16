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
		log_info("CollisionSystem");

		if (tagId == eTag::FOOD_TAG) {
			log_info("FOOD_TAG::FoodCollision");

			getWorld().getUnivers().playNoise(eSound::FOOD);

			//delete
			log_info("FOOD_TAG::FoodCollision2");
			log_warn("Head collision x[%d] y[%d] id[%d]",snakePositionComponent.x, snakePositionComponent.y, entityHead.getGroupIdByEntity());
			log_info("FOOD_TAG::FoodCollision3");
			entityCheck.kill();
			getWorld().getEventsManager().emitEvent<FoodEat>(entityHead.getGroupIdByEntity());

			if (getWorld().getUnivers().getGameNetwork()->getId() == entityHead.getGroupIdByEntity() ||
				getWorld().getUnivers().isIASnake(entityHead.getGroupIdByEntity())) {

				ClientTCP::FoodInfo foodInfo(PositionComponent(
						getWorld().grid.getRandomSlot(eSprite::NONE)), false);
				getWorld().getUnivers().getGameNetwork()->write_socket(
						ClientTCP::add_prefix(eHeader::FOOD, &foodInfo));
			}
		} else if (tagId == eTag::FOOD_TAG_FROM_SNAKE) {
			log_info("FOOD_TAG_FROM_SNAKE::FoodCollision");
			getWorld().getUnivers().playNoise(eSound::FOOD);
			entityCheck.kill();
			getWorld().getEventsManager().emitEvent<FoodEat>(entityHead.getGroupIdByEntity());
		} else if (tagId == WALL_TAG) {
			log_info("WALL_TAG::WallCollision1");
			if (entityHead.getGroupIdByEntity() ==
						getWorld().getUnivers().getGameNetwork()->getId() ||
					getWorld().getUnivers().isIASnake(entityHead.getGroupIdByEntity()))
				getWorld().getUnivers().getGameNetwork()->killSnake(entityHead.getGroupIdByEntity());
			log_info("WALL_TAG::WallCollision2");
			entityHead.killGroup();
			log_info("WALL_TAG::WallCollision3");
		} else if (entityCheck.getGroupIdByEntity() == entityHead.getGroupIdByEntity()) {
			log_info("HIMSELF::CollisionOnHimself1");
			if (entityHead.getGroupIdByEntity() ==
						getWorld().getUnivers().getGameNetwork()->getId() ||
					getWorld().getUnivers().isIASnake(entityHead.getGroupIdByEntity()))
				getWorld().getUnivers().getGameNetwork()->killSnake(entityHead.getGroupIdByEntity());
			log_info("HIMSELF::CollisionOnHimself2");
			entityHead.killGroup();
			log_info("HIMSELF::CollisionOnHimself3");
		} else {
			log_info("SNAKE::CollisionSnake");

			auto appleSnake = getWorld().getEntitiesManager().getEntitiesByGroupId(entityHead.getGroupIdByEntity());
			for (auto snake : appleSnake) {
				ClientTCP::FoodInfo foodInfo(snake.getComponent<PositionComponent>(), true);
				if (snake.hasComponent<PositionComponent>())
					getWorld().getUnivers().getGameNetwork()->write_socket(
							ClientTCP::add_prefix(eHeader::FOOD, &foodInfo));
			}
			if (entityHead.getGroupIdByEntity() ==
						getWorld().getUnivers().getGameNetwork()->getId() ||
					getWorld().getUnivers().isIASnake(entityHead.getGroupIdByEntity()))
				getWorld().getUnivers().getGameNetwork()->killSnake(entityHead.getGroupIdByEntity());

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
			log_info("SNAKE::update");

			for (auto entityCheck : entities_) {
				checkCollision(entity, entityCheck);
			}
		}
	}
}


