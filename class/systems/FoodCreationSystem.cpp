#include <component/CollisionComponent.hpp>
#include "FoodCreationSystem.hpp"
#include <KINU/World.hpp>
#include <events/FoodEat.hpp>
#include <component/FollowComponent.hpp>
#include <component/SpriteComponent.hpp>
#include <factory/Factory.hpp>
#include <network/ClientTCP.hpp>
#include <systems/SpriteSystem.hpp>
#include <logger.h>

FoodCreationSystem::FoodCreationSystem() {
	requireComponent<FollowComponent>();
	requireComponent<PositionComponent>();
}

void FoodCreationSystem::update() {
	log_success("update");

	auto foodCreationEvents = getWorld().getEventManager().getEvents<FoodCreation>();
	for (auto foodCreationEvent : foodCreationEvents) {
		auto food = getWorld().createEntity();
		food.addComponent(foodCreationEvent.positionComponent_);
		food.addComponent<CollisionComponent>();
		food.addComponent<SpriteComponent>(eSprite::FOOD, NO_PRIORITY);
		food.group(FOOD_TAG);
	}
}

//
// {H:0:1,1} {B:1:1,2:S[0]} {B:2:1,3:S[1]} 	{B:4:1,4:S[2]} {T:3:1,4:S[4]}
// {H:0:1,1} {B:1:1,2:S[0]} {B:2:1,3:S[1]} 	{B:4:1,3:S[2]} {T:3:1,4:S[4]}