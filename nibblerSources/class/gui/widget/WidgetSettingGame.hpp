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

private:
	unsigned int mapSize_;

	void content_(bool renderContentInWindow) override;
};
