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
	MutantGrid< eSprite > &grid = getWorld().grid;
	MutantGrid< eSprite > grid_cache(getWorld().getUnivers().getMapSize());
	grid_cache.fill(eSprite::NONE);

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

	for (auto &renderComponent : renderComponents) {
		grid_cache(renderComponent.first.x, renderComponent.first.y) = renderComponent.second.sprite;
	}
	getWorld().grid = grid_cache;
}



