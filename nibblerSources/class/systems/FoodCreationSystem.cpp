#include <component/CollisionComponent.hpp>
#include "FoodCreationSystem.hpp"
#include <KINU/World.hpp>
#include <component/FollowComponent.hpp>
#include <component/SpriteComponent.hpp>
#include <systems/SpriteSystem.hpp>
#include <logger.h>
#include <events/FoodCreation.hpp>

FoodCreationSystem::FoodCreationSystem() {
}

void FoodCreationSystem::update() {
	log_info("%s", __PRETTY_FUNCTION__);
	auto foodCreationEvents = getWorld().getEventsManager().getEvents<FoodCreation>();

	for (auto foodCreationEvent : foodCreationEvents) {
		auto food = getWorld().createEntity();
		log_info("FoodCreationSystem:: x[%d] y[%d]", foodCreationEvent.positionComponent_.x, foodCreationEvent.positionComponent_.y);
		food.addComponent(foodCreationEvent.positionComponent_);
		food.addComponent<CollisionComponent>();
		food.addComponent<SpriteComponent>(eSprite::kFood, kNoPriority);
		food.groupEntityByGroupId((foodCreationEvent.fromSnake_ ? eTag::kFoodFromSnake : eTag::kFoodTag));
	}
}

//
// {H:0:1,1} {B:1:1,2:S[0]} {B:2:1,3:S[1]} 	{B:4:1,4:S[2]} {T:3:1,4:S[4]}
// {H:0:1,1} {B:1:1,2:S[0]} {B:2:1,3:S[1]} 	{B:4:1,3:S[2]} {T:3:1,4:S[4]}