#include <component/CollisionComponent.hpp>
#include "FoodSystem.hpp"
#include <KINU/World.hpp>
#include <events/FoodEat.hpp>
#include <component/FollowComponent.hpp>
#include <component/SpriteComponent.hpp>
#include <factory/Factory.hpp>
#include <network/ClientTCP.hpp>
#include <systems/SpriteSystem.hpp>
#include <logger.h>

FoodSystem::FoodSystem() {
	requireComponent<FollowComponent>();
	requireComponent<PositionComponent>();
}

void FoodSystem::update() {
	log_success("update");

	auto events = getWorld().getEventManager().getEvents<FoodEat>();

	for (auto &event : events) {
		auto entityTail = getWorld().getEntityManager()
				.getEntityByTag(Factory::factory_name(TAIL, event.id_));
		auto newEntity = getWorld().createEntity();

		// Position == entityTail.positionComponent
		newEntity.addComponent<PositionComponent>(
				entityTail.getComponent<PositionComponent>());

		//FollowComponent.id == entityTail.idFollowed
		newEntity.addComponent<FollowComponent>(
				entityTail.getComponent<FollowComponent>().idFollowed, false);
		newEntity.addComponent<CollisionComponent>();


		// NEW : Make TO_direction
		KINU::Entity entityFollowed = getWorld().getEntityManager().getEntity(
				entityTail.getComponent<FollowComponent>().idFollowed);

		eSprite sprite = eSprite::BODY |
						 (getWorld().getUnivers().getClientTCP_().getSnakes()[std::stoi(entityTail.getTag())].sprite &
						  eSprite::MASK_COLOR)
						 | (SpriteSystem::spriteDirection(
				newEntity.getComponent<PositionComponent>(),
				entityFollowed.getComponent<PositionComponent>())
				<< eSprite::BITWISE_TO);
		log_warn("New entity create sprite [%d]", sprite);
		newEntity.addComponent<SpriteComponent>(sprite, MINOR_PRIORITY);

		auto &followComponent = entityTail.getComponent<FollowComponent>();
		followComponent.idFollowed = newEntity.getIndex();
		followComponent.skip = true;

		newEntity.tag(Factory::factory_name(BODY, newEntity.getIndex()));
		newEntity.group(entityTail.getGroup());
	}

	auto foodCreationEvents = getWorld().getEventManager().getEvents<FoodCreation>();
	for (auto foodCreationEvent : foodCreationEvents) {
		auto food = getWorld().createEntity();
		food.addComponent(foodCreationEvent.positionComponent_);
		food.addComponent<CollisionComponent>(false);
		food.addComponent<SpriteComponent>(eSprite::FOOD, NO_PRIORITY);
		food.group(FOOD_TAG);
	}
}

//
// {H:0:1,1} {B:1:1,2:S[0]} {B:2:1,3:S[1]} 	{B:4:1,4:S[2]} {T:3:1,4:S[4]}
// {H:0:1,1} {B:1:1,2:S[0]} {B:2:1,3:S[1]} 	{B:4:1,3:S[2]} {T:3:1,4:S[4]}