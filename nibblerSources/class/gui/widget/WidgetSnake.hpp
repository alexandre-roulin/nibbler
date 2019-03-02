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

	WidgetSnake(Gui &core, Snake const &snake, std::map<eSprite, SpriteColorProperties> const &mapSprite,
				SnakeType type);
	~WidgetSnake() override = default;
	WidgetSnake &operator=(const WidgetSnake &) = delete;
	WidgetSnake(const WidgetSnake &) = delete;
	WidgetSnake() = delete;

private:
	Snake const &snake_;
	std::map<eSprite, SpriteColorProperties> const &mapSprite_;
	SnakeType type_;
	unsigned int sizeTexture_;

	void content_(bool renderContentInWindow) override;

	void renderIa_();

	void renderYourSnake_();

	void renderOtherSnake_();

	void renderStaticDataSnake_() const;

	void renderName_() const;

	void renderImage_() const;

	void updateSizeTexture_();

	void renderSelectionColor_() const;
};
