#pragma once

#include "AWidget.hpp"
#include <string>
#include <SFML/Graphics.hpp>
#include "WidgetSnake.hpp"
#include "nibbler.hpp"

class Gui;

class WidgetMassiveButton : public AWidget {
public:
	WidgetMassiveButton(Gui &core);

	~WidgetMassiveButton(void) override = default;

	void render(void) override;

private:

	WidgetMassiveButton &operator=(const WidgetMassiveButton &) = default;

	WidgetMassiveButton(const WidgetMassiveButton &) = default;
};
