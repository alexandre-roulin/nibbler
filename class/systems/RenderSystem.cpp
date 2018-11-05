#include <component/PositionComponent.hpp>
#include <component/SpriteComponent.hpp>
#include "RenderSystem.hpp"
#include "KNU/World.hpp"

RenderSystem::RenderSystem() {
	requireComponent<PositionComponent>();
	requireComponent<SpriteComponent>();

}

void RenderSystem::update() {
	for (auto &entity : getEntities()) {
		auto& positionComponent = entity.getComponent<PositionComponent>();
		auto& spriteComponent = entity.getComponent<SpriteComponent>();
//		getWorld().getDisplay()->drawTileGrid(spriteComponent.sprite,
//				positionComponent.y, positionComponent.x);
	}
}
