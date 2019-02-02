#include "CollisionSystem.hpp"
#include <factory/Factory.hpp>
#include <events/FoodEat.hpp>
#include <network/ClientTCP.hpp>
#include <logger.h>
#include <MutantGrid.tpp>
#include <component/CollisionComponent.hpp>
#include <KINU/Entity.hpp>
#include <KINU/World.hpp>
#include <network/Data.hpp>
#include <network/SnakeClient.hpp>


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

			if (getWorld().getUnivers().getSnakeClient()->getId_() == entityHead.getGroupIdByEntity() ||
				getWorld().getUnivers().isIASnake(entityHead.getGroupIdByEntity())) {

				getWorld().getUnivers().getSnakeClient()->sendDataToServer(
						FoodInfo(PositionComponent(
								getWorld().grid.getRandomSlot(eSprite::NONE)),
										false),
										eHeaderK::kFood);
			}
		} else if (tagId == eTag::FOOD_TAG_FROM_SNAKE) {
			getWorld().getUnivers().playNoise(eSound::FOOD);
			entityCheck.kill();
			if (entityHead.hasGroupId())
				getWorld().getEventsManager().emitEvent<FoodEat>(entityHead.getGroupIdByEntity());
		} else if (tagId == WALL_TAG) {
			createAppleBySnake(entityHead);
			getWorld().getUnivers().getSnakeClient()->killSnake(entityHead.getGroupIdByEntity());
			entityHead.killGroup();
		} else if (entityCheck.getGroupIdByEntity() == entityHead.getGroupIdByEntity()) {
			createAppleBySnake(entityHead);
			getWorld().getUnivers().getSnakeClient()->killSnake(entityHead.getGroupIdByEntity());
			entityHead.killGroup();
		} else {
			createAppleBySnake(entityHead);
			getWorld().getUnivers().getSnakeClient()->killSnake(entityHead.getGroupIdByEntity());
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
	for (auto snakeCheck : appleSnake) {
		if (snakeCheck.hasComponent<PositionComponent>()) {
			auto positionComponent = snakeCheck.getComponent<PositionComponent>();
			if (positionComponent != positionHead) {
				getWorld().getUnivers().getSnakeClient()->sendDataToServer(
						FoodInfo(positionComponent, true), eHeaderK::kFood);
			}
		}
	}
}
