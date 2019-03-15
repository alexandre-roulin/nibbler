#include "KINU/World.hpp"
#include "RenderSystem.hpp"
#include <component/SpriteComponent.hpp>
#include <list>
#include <iostream>

RenderSystem::RenderSystem(Univers &univers) : univers_(univers) {
	requireComponent<PositionComponent>();
	requireComponent<SpriteComponent>();
}

void RenderSystem::update() {
	std::list<std::pair<PositionComponent &, SpriteComponent &>> renderComponents;
	MutantGrid< eSprite > grid_cache(univers_.getMapSize());
	grid_cache.fill(eSprite::kNone);

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

	int mapSize = univers_.getMapSize();
	for (auto &renderComponent : renderComponents) {
		if (renderComponent.first.x >= 0 && renderComponent.first.x < mapSize && renderComponent.first.y >= 0 && renderComponent.first.y < mapSize)
			grid_cache(renderComponent.first.x, renderComponent.first.y) = renderComponent.second.sprite;
	}
	univers_.getGrid_() = grid_cache;
}



