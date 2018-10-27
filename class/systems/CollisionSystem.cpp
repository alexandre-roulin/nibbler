#include <component/PositionComponent.hpp>
#include <component/CollectComponent.hpp>
#include "CollisionSystem.hpp"
#include "KNU/World.hpp"
#include <events/FoodEvent.hpp>

CollisionSystem::CollisionSystem() {
	requireComponent<CollectComponent>();
	requireComponent<PositionComponent>();
}

void CollisionSystem::update() {
	std::vector<KNU::Entity> v = getEntities();
	auto &world = getWorld();
	auto snake_head = getWorld().getEntityManager()
			.getEntityByTag(TAG_HEAD_SNAKE);
	if (std::any_of(v.begin(), v.end(), [snake_head](KNU::Entity &entity) {

		if (entity != snake_head &&
			(snake_head.getComponent<PositionComponent>() ==
			 entity.getComponent<PositionComponent>())) {
			log_success("update [FOOD]");
			entity.kill();
			return true;
		}
		return false;
	})) {
		world.getEventManager().emitEvent<FoodEvent>(false);
	}
}
