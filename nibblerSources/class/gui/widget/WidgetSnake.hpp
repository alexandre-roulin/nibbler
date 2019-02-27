#pragma once

#include "AWidget.hpp"
#include "SpriteColorProperties.hpp"
#include <vector>
#include "nibbler.hpp"
#include <cores/Snake.hpp>

class Gui;

class WidgetSnake : public AWidget {
public:

	enum SnakeType { kBasic, kYour, kIa};

	WidgetSnake(Gui &core, SnakeUI const &snake, std::map<eSprite, SpriteColorProperties> const &mapSprite,
				SnakeType type);

	~WidgetSnake(void) override = default;

	WidgetSnake &operator=(const WidgetSnake &) = delete;

	WidgetSnake(const WidgetSnake &) = delete;

	WidgetSnake(void) = delete;

	void render(void) override ;

private:
	SnakeUI const &snake_;
	std::map<eSprite, SpriteColorProperties> const &mapSprite_;
	SnakeType type_;
	unsigned int sizeTexture_;

	void renderIa_(void);

	void renderYourSnake_(void);

	void renderOtherSnake_(void);

	void renderStaticDataSnake_() const;

	void renderName_() const;

	void renderImage_() const;

	void updateSizeTexture_(void);

	void renderSelectionColor_() const;
};
