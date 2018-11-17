#include <component/CollisionComponent.hpp>
#include "FoodSystem.hpp"
#include <KINU/World.hpp>
#include <events/FoodEat.hpp>
#include <component/FollowComponent.hpp>
#include <component/SpriteComponent.hpp>
#include <factory/Factory.hpp>
#include <network/ClientTCP.hpp>

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
		newEntity.addComponent<PositionComponent>(entityTail.getComponent<PositionComponent>());

		//FollowComponent.id == entityTail.idFollowed
		newEntity.addComponent<FollowComponent>(entityTail.getComponent<FollowComponent>().idFollowed, false);
		newEntity.addComponent<SpriteComponent>(36);
		newEntity.addComponent<CollisionComponent>();

		auto &followComponent = entityTail.getComponent<FollowComponent>();
		followComponent.idFollowed = newEntity.getIndex();
		followComponent.skip = true;


		newEntity.tag(Factory::factory_name(BODY, newEntity.getIndex()));
		newEntity.group(entityTail.getGroup());
		createFood();
	}

	auto foodCreationEvents = getWorld().getEventManager().getEvents<FoodCreation>();
	log_warn("Find %d foodCreationEvents",foodCreationEvents.size());

	for (auto foodCreationEvent : foodCreationEvents) {
		auto food = getWorld().createEntity();
		food.addComponent<PositionComponent>(foodCreationEvent.y, foodCreationEvent.x);
		food.addComponent<CollisionComponent>(false);
		food.addComponent<SpriteComponent>(0);
		food.group(FOOD_TAG);
	}
}

void FoodSystem::createFood() {
	int position[2] = {(rand() % (30 - 2)) + 1,
					   (rand() % (30 - 2)) + 1};
	getWorld().getUnivers()
			.getClientTCP_()
			.write_socket(ClientTCP::add_prefix(FOOD, position));
}

//
// {H:0:1,1} {B:1:1,2:S[0]} {B:2:1,3:S[1]} 	{B:4:1,4:S[2]} {T:3:1,4:S[4]}
// {H:0:1,1} {B:1:1,2:S[0]} {B:2:1,3:S[1]} 	{B:4:1,3:S[2]} {T:3:1,4:S[4]}