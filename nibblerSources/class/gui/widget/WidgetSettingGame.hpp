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
	const GameManager::eSpeed &speed;

	void update_() override;
	void beginContent_() override;

	const std::string &getDifficultyDisplay_(const GameManager::eSpeed speed);
	GameManager::eSpeed getSpeedEnum(const std::string &string);

	static const std::array<std::string, 4> difficultyDisplay;
};
