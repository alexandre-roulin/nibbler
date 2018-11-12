#include <component/CollisionComponent.hpp>
#include "FoodSystem.hpp"
#include <KINU/World.hpp>
#include <events/FoodEvent.hpp>
#include <component/FollowComponent.hpp>
#include <component/SpriteComponent.hpp>
#include <factory/Factory.hpp>
#include <Univers.hpp>
#include <network/ClientTCP.hpp>
#include <logger.h>

FoodSystem::FoodSystem() {
	requireComponent<FollowComponent>();
	requireComponent<PositionComponent>();
}

void FoodSystem::update() {
	log_success("update");

	auto events = getWorld().getEventManager().getEvents<FoodEvent>();

	for (auto &event : events) {
		if (!event.tag_tail.empty()) {
			auto entityTail = getWorld().getEntityManager().getEntityByTag(event.tag_tail);
			auto newEntity = getWorld().createEntity();

			auto &followComponent = entityTail.getComponent<FollowComponent>();        //SNAKE TAIL WITH ID FOLLOW
			auto &positionComponent = entityTail.getComponent<PositionComponent>();    //SNAKE TAIL WITH ID FOLLOW


			newEntity.addComponent<PositionComponent>(
					positionComponent);            // Position == entityTail.positionComponent
			newEntity.addComponent<FollowComponent>(
					followComponent.idFollowed); //FollowComponent.id == entityTail.idFollowed
			newEntity.addComponent<SpriteComponent>(36);
			followComponent.idFollowed = newEntity.getId();
			auto &followComponent2 = entityTail.getComponent<FollowComponent>();        //SNAKE TAIL WITH ID FOLLOW
			followComponent.skip = true;
			newEntity.tag(Factory::factory_name(BODY, newEntity.getId()));
			newEntity.group(event.tag_group);
			followComponent.skip = true;
			createFood();
		}
	}
}

void FoodSystem::createFood() {
	int position[2] = {(rand() % (getWorld().getMax_() - 2)) + 1,
					   (rand() % (getWorld().getMax_() - 2)) + 1};

	getWorld().getUnivers()
			.getClientTCP_()
			.write_socket(ClientTCP::add_prefix(FOOD, position));
}

//
// {H:0:1,1} {B:1:1,2:S[0]} {B:2:1,3:S[1]} 	{B:4:1,4:S[2]} {T:3:1,4:S[4]}
// {H:0:1,1} {B:1:1,2:S[0]} {B:2:1,3:S[1]} 	{B:4:1,3:S[2]} {T:3:1,4:S[4]}