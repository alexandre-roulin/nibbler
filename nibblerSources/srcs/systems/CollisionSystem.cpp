#include "CollisionSystem.hpp"
#include <factory/Factory.hpp>
#include <events/FoodEat.hpp>
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

		if (tagId == eTag::kFoodTag) {
			univers_.getSoundManager().playNoise(eNoise::kFoodSound);
			entitiesToKill.insert(entityCheck);
			getWorld().getEventsManager().emitEvent<FoodEat>(entityHead.getGroupIdByEntity());

			if (ptr && (ptr->getId_() == entityHead.getGroupIdByEntity() ||
						univers_.isIASnake(entityHead.getGroupIdByEntity()))) {
				ptr->sendDataToServer(
						FoodInfo(PositionComponent(
								univers_.getGrid_().getRandomSlot(eSprite::kNone)),
										false,
										entityHead.getGroupIdByEntity()),
										eHeader::kFood);
			}
		} else if (tagId == eTag::kFoodFromSnake) {

			univers_.getSoundManager().playNoise(eNoise::kFoodSound);
			entitiesToKill.insert(entityCheck);
			if (entityHead.hasGroupId())
				getWorld().getEventsManager().emitEvent<FoodEat>(entityHead.getGroupIdByEntity());
		} else {
			createAppleBySnake(entityHead);
			ptr->killSnake(entityHead.getGroupIdByEntity());
			auto entities = getWorld().getEntitiesManager().getEntitiesByGroupId(entityHead.getGroupIdByEntity());

			for (auto entity : entities) {
				entitiesToKill.insert(entity);
			}
		}

	}
}

void CollisionSystem::update() {
	std::vector<KINU::Entity> entities_ = getEntities();
	for (auto entity : getEntities()) {
		if (entity.hasTagId() &&
			entity.getTagId() - entity.getGroupIdByEntity() == eTag::kHeadTag) {
			for (auto entityCheck : entities_) {
				checkCollision(entity, entityCheck);
			}
		}
	}

	if (!entitiesToKill.empty()) {
		std::for_each(entitiesToKill.begin(), entitiesToKill.end(), [](KINU::Entity e){
			e.kill();
		});
		entitiesToKill.clear();
	}
}

void CollisionSystem::createAppleBySnake(KINU::Entity snake) {

	SnakeClient::boost_shared_ptr ptr(univers_.getSnakeClient().lock());

	if ((ptr && ptr->getId_() == snake.getGroupIdByEntity()) || univers_.isOnlyIA()) {
		auto appleSnake = getWorld().getEntitiesManager().getEntitiesByGroupId(snake.getGroupIdByEntity());
		auto positionHead = snake.getComponent<PositionComponent>();
		for (auto snakeCheck : appleSnake) {
			if (snakeCheck.hasComponent<PositionComponent>()) {
				auto positionComponent = snakeCheck.getComponent<PositionComponent>();
				if (positionComponent != positionHead && ptr) {
					ptr->sendDataToServer(FoodInfo(positionComponent, true, -1), eHeader::kFood);
				}
			}
		}
	}

}

