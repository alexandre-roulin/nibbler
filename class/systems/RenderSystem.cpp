#include "KINU/World.h"
#include "RenderSystem.hpp"
#include <component/PositionComponent.hpp>
#include <component/SpriteComponent.hpp>
#include <logger.h>

RenderSystem::RenderSystem() {
	requireComponent<PositionComponent>();
	requireComponent<SpriteComponent>();
}

void RenderSystem::update() {
	log_success("update");
	for (auto &entity : getEntities()) {
		if (entity.isAlive()) {
			auto& positionComponent = entity.getComponent<PositionComponent>();
			auto& spriteComponent = entity.getComponent<SpriteComponent>();
			getWorld().grid(positionComponent.x, positionComponent.y)
					= spriteComponent.sprite;
		}
	}
}
