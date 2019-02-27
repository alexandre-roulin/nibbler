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
		KINU::Entity entityHead,
		KINU::Entity entityCheck) {
	SnakeClient::boost_shared_ptr ptr(univers_.getSnakeClient().lock());


	auto &snakePositionComponent = entityHead.getComponent<PositionComponent>();
	auto &positionComponent = entityCheck.getComponent<PositionComponent>();

	if (ptr && snakePositionComponent == positionComponent &&
		entityHead != entityCheck && entityCheck.hasGroupId() && entityHead.hasGroupId()) {
		KINU::TagId tagId = entityCheck.getGroupIdByEntity();
		log_info("CollisionSystem");

		if (tagId == eTag::kFoodTag) {
			log_info("FOOD_TAG::FoodCollision");
			univers_.getSoundManager().playNoise(eNoise::kFoodSound);
			entityCheck.kill();
			getWorld().getEventsManager().emitEvent<FoodEat>(entityHead.getGroupIdByEntity());

			if (ptr && (ptr->getId_() == entityHead.getGroupIdByEntity() ||
						univers_.isIASnake(entityHead.getGroupIdByEntity()))) {
				ptr->addScore(entityHead.getGroupIdByEntity(), eScore::kFromFood);
				ptr->sendDataToServer(
						FoodInfo(PositionComponent(
								univers_.getGrid_().getRandomSlot(eSprite::kNone)),
										false),
										eHeader::kFood);
			}
		} else if (tagId == eTag::kFoodFromSnake) {

			univers_.getSoundManager().playNoise(eNoise::kFoodSound);
			entityCheck.kill();
			if (ptr && (ptr->getId_() == entityHead.getGroupIdByEntity() ||
						univers_.isIASnake(entityHead.getGroupIdByEntity())))
				ptr->addScore(entityHead.getGroupIdByEntity(), eScore::kFromFood);
			if (entityHead.hasGroupId())
				getWorld().getEventsManager().emitEvent<FoodEat>(entityHead.getGroupIdByEntity());
		} else if (tagId == kWallTag) {
			createAppleBySnake(entityHead);
			ptr->killSnake(entityHead.getGroupIdByEntity());
			entityHead.killGroup();
		} else if (entityCheck.getGroupIdByEntity() == entityHead.getGroupIdByEntity()) {
			createAppleBySnake(entityHead);
			ptr->killSnake(entityHead.getGroupIdByEntity());
			entityHead.killGroup();
		} else {
			createAppleBySnake(entityHead);
			ptr->killSnake(entityHead.getGroupIdByEntity());
			entityHead.killGroup();
		}

	}
}

void CollisionSystem::update() {
//	log_info("%s", __PRETTY_FUNCTION__);
	std::vector<KINU::Entity> entities_ = getEntities();
	for (auto entity : getEntities()) {
		if (entity.hasTagId() &&
			entity.getTagId() - entity.getGroupIdByEntity() == eTag::kHeadTag) {
			for (auto entityCheck : entities_) {
				checkCollision(entity, entityCheck);
			}
		}
	}
}

void CollisionSystem::createAppleBySnake(KINU::Entity snake) {

	SnakeClient::boost_shared_ptr ptr(univers_.getSnakeClient().lock());

	auto appleSnake = getWorld().getEntitiesManager().getEntitiesByGroupId(snake.getGroupIdByEntity());
	auto positionHead = snake.getComponent<PositionComponent>();
	for (auto snakeCheck : appleSnake) {
		if (snakeCheck.hasComponent<PositionComponent>()) {
			auto positionComponent = snakeCheck.getComponent<PositionComponent>();
			if (positionComponent != positionHead && ptr) {
				ptr->sendDataToServer(
						FoodInfo(positionComponent, true), eHeader::kFood);
			}
		}
	}
}
