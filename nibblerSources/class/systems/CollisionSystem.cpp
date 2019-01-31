#include "CollisionSystem.hpp"
#include <factory/Factory.hpp>
#include <events/FoodEat.hpp>
#include <network/ClientTCP.hpp>
#include <logger.h>
#include <MutantGrid.tpp>
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
		entityHead != entityCheck && entityCheck.hasGroupId() && entityHead.hasGroupId()) {
		auto &collisionComponent = entityCheck.getComponent<CollisionComponent>();

		KINU::TagId tagId = entityCheck.getGroupIdByEntity();
		log_info("CollisionSystem");

		if (tagId == eTag::FOOD_TAG) {
			log_info("FOOD_TAG::FoodCollision");
			getWorld().getUnivers().playNoise(eSound::FOOD);
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
			if (entityHead.hasGroupId())
				getWorld().getEventsManager().emitEvent<FoodEat>(entityHead.getGroupIdByEntity());
		} else if (tagId == WALL_TAG) {
			log_info("WALL_TAG::WallCollision");
			createAppleBySnake(entityHead);
			if (entityHead.getGroupIdByEntity() ==
						getWorld().getUnivers().getGameNetwork()->getId() ||
					getWorld().getUnivers().isIASnake(entityHead.getGroupIdByEntity()))
				getWorld().getUnivers().getGameNetwork()->killSnake(entityHead.getGroupIdByEntity());
			entityHead.killGroup();
		} else if (entityCheck.getGroupIdByEntity() == entityHead.getGroupIdByEntity()) {
			createAppleBySnake(entityHead);
			log_info("HIMSELF::CollisionOnHimself");
			if (entityHead.getGroupIdByEntity() ==
						getWorld().getUnivers().getGameNetwork()->getId() ||
					getWorld().getUnivers().isIASnake(entityHead.getGroupIdByEntity()))
				getWorld().getUnivers().getGameNetwork()->killSnake(entityHead.getGroupIdByEntity());
			entityHead.killGroup();
		} else {
			log_info("SNAKE::CollisionSnake");

			createAppleBySnake(entityHead);
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
			for (auto entityCheck : entities_) {
				checkCollision(entity, entityCheck);
			}
		}
	}
}

void CollisionSystem::createAppleBySnake(KINU::Entity snake) {

	auto appleSnake = getWorld().getEntitiesManager().getEntitiesByGroupId(snake.getGroupIdByEntity());
	auto positionHead = snake.getComponent<PositionComponent>();
	for (auto snake : appleSnake) {
		if (snake.hasComponent<PositionComponent>()) {
			auto positionComponent = snake.getComponent<PositionComponent>();
			if (positionComponent != positionHead) {
				ClientTCP::FoodInfo foodInfo(positionComponent, true);
				getWorld().getUnivers().getGameNetwork()->write_socket(
						ClientTCP::add_prefix(eHeader::FOOD, &foodInfo));
			}
		}
	}
}
