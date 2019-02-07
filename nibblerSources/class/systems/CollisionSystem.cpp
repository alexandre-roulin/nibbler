#include "CollisionSystem.hpp"
#include <factory/Factory.hpp>
#include <events/FoodEat.hpp>
#include <logger.h>
#include <MutantGrid.tpp>
#include <component/CollisionComponent.hpp>
#include <KINU/Entity.hpp>
#include <KINU/World.hpp>
#include <network/Data.hpp>
#include <network/SnakeClient.hpp>


CollisionSystem::CollisionSystem(Univers &univers) : univers_(univers) {
	requireComponent<CollisionComponent>();
	requireComponent<PositionComponent>();
}


void CollisionSystem::checkCollision(
		KINU::Entity entityHead, KINU::Entity entityCheck) {

	auto &snakePositionComponent = entityHead.getComponent<PositionComponent>();
	auto &positionComponent = entityCheck.getComponent<PositionComponent>();

	if (snakePositionComponent == positionComponent &&
		entityHead != entityCheck && entityCheck.hasGroupId() && entityHead.hasGroupId()) {
		KINU::TagId tagId = entityCheck.getGroupIdByEntity();
		log_info("CollisionSystem");

		if (tagId == eTag::FOOD_TAG) {
			log_info("FOOD_TAG::FoodCollision");
			univers_.playNoise(eNoise::FOOD);
			entityCheck.kill();
			getWorld().getEventsManager().emitEvent<FoodEat>(entityHead.getGroupIdByEntity());

			if (univers_.getSnakeClient()->getId_() == entityHead.getGroupIdByEntity() ||
				univers_.isIASnake(entityHead.getGroupIdByEntity())) {

				univers_.getSnakeClient()->sendDataToServer(
						FoodInfo(PositionComponent(
								univers_.getGrid_().getRandomSlot(eSprite::NONE)),
										false),
										eHeaderK::kFood);
			}
		} else if (tagId == eTag::FOOD_TAG_FROM_SNAKE) {
			univers_.playNoise(eNoise::FOOD);
			entityCheck.kill();
			if (entityHead.hasGroupId())
				getWorld().getEventsManager().emitEvent<FoodEat>(entityHead.getGroupIdByEntity());
		} else if (tagId == WALL_TAG) {
			createAppleBySnake(entityHead);
			univers_.getSnakeClient()->killSnake(entityHead.getGroupIdByEntity());
			entityHead.killGroup();
		} else if (entityCheck.getGroupIdByEntity() == entityHead.getGroupIdByEntity()) {
			createAppleBySnake(entityHead);
			univers_.getSnakeClient()->killSnake(entityHead.getGroupIdByEntity());
			entityHead.killGroup();
		} else {
			createAppleBySnake(entityHead);
			univers_.getSnakeClient()->killSnake(entityHead.getGroupIdByEntity());
			entityHead.killGroup();
		}

	}
}

void CollisionSystem::update() {
	std::vector<KINU::Entity> entities_ = getEntities();
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
				univers_.getSnakeClient()->sendDataToServer(
						FoodInfo(positionComponent, true), eHeaderK::kFood);
			}
		}
	}
}
