#pragma once

#include "AWidget.hpp"
#include "SpriteColorProperties.hpp"
#include <vector>
#include "nibbler.hpp"
#include <cores/Snake.hpp>

class Core;

class WidgetSnake : public AWidget {
public:
	WidgetSnake(Core &core, Snake const &snake, std::map<eSprite, SpriteColorProperties> const &mapSprite,
				bool isYourSnake = false);

	~WidgetSnake(void) override = default;

	void render(void) override ;

private:
	Snake const &snake_;
	std::map<eSprite, SpriteColorProperties> const &mapSprite_;
	bool isYourSnake_;

	void renderYourSnake_(void);

	void renderOtherSnake_(void);

	void renderName_(unsigned int sizeTexture) const;

	void renderImage_(unsigned int sizeTexture) const;

	unsigned int sizeTexture_(void) const;

	void renderSelectionColor_(unsigned int sizeTexture) const;

	WidgetSnake &operator=(const WidgetSnake &) = delete;

	WidgetSnake(const WidgetSnake &) = delete;

	WidgetSnake(void) = delete;
};
