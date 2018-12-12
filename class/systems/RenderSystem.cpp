#include <logger.h>
#include "KINU/World.hpp"
#include "RenderSystem.hpp"
#include <component/SpriteComponent.hpp>
#include <list>
#include <iostream>

RenderSystem::RenderSystem() {
	requireComponent<PositionComponent>();
	requireComponent<SpriteComponent>();
}

void RenderSystem::update() {
	std::list<std::pair<PositionComponent &, SpriteComponent &>> renderComponents;
	Grid< eSprite > &grid = getWorld().grid;


	for (auto &entity : getEntities()) {
		renderComponents.push_back(
				std::pair<PositionComponent &, SpriteComponent &>(
						entity.getComponent<PositionComponent>(),
						entity.getComponent<SpriteComponent>()));
	}
	renderComponents.sort(
			[](auto const &renderPair1, auto const &renderPair2) -> bool {
				return renderPair1.second.priority <
					   renderPair2.second.priority;
			});

	grid.fill(eSprite::NONE);
	for (auto &renderComponent : renderComponents) {
		grid(renderComponent.first.x, renderComponent.first.y) = renderComponent.second.sprite;
	}
}



