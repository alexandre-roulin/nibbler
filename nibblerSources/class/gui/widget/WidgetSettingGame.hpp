#pragma once

#include "AWidget.hpp"
#include "nibbler.hpp"

class Gui;

class WidgetSettingGame : public AWidget {
public:
	WidgetSettingGame(Gui &);
	~WidgetSettingGame() override = default;
	WidgetSettingGame() = delete;
	WidgetSettingGame &operator=(const WidgetSettingGame &) = delete;
	WidgetSettingGame(const WidgetSettingGame &) = delete;

	void render(void) override;

private:
	unsigned int mapSize_;
};
