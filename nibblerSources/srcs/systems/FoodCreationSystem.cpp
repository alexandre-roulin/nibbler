#include <component/CollisionComponent.hpp>
#include "FoodCreationSystem.hpp"
#include <KINU/World.hpp>
#include <component/FollowComponent.hpp>
#include <component/SpriteComponent.hpp>
#include <systems/SpriteSystem.hpp>
#include <events/FoodCreation.hpp>

FoodCreationSystem::FoodCreationSystem(unsigned int mapSize): mapSize_(mapSize) {
}

void FoodCreationSystem::update() {
	auto foodCreationEvents = getWorld().getEventsManager().getEvents<FoodCreation>();

	for (auto foodCreationEvent : foodCreationEvents) {
		auto food = getWorld().createEntity();
		if (foodCreationEvent.positionComponent_.y >= 0 && foodCreationEvent.positionComponent_.y < mapSize_ &&
				foodCreationEvent.positionComponent_.x >= 0 && foodCreationEvent.positionComponent_.x < mapSize_) {

			food.addComponent(foodCreationEvent.positionComponent_);
			food.addComponent<CollisionComponent>();
			food.addComponent<SpriteComponent>(eSprite::kFood, kNoPriority);
			food.groupEntityByGroupId((foodCreationEvent.fromSnake_ ? eTag::kFoodFromSnake : eTag::kFoodTag));
		}
	}
}