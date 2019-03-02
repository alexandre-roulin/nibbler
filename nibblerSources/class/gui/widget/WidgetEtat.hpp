#pragma once

#include "AWidget.hpp"
#include "nibbler.hpp"

class Gui;

class WidgetEtat : public AWidget {
public:
	WidgetEtat(Gui &);
	~WidgetEtat() override = default;
	WidgetEtat() = delete;
	WidgetEtat &operator=(const WidgetEtat &) = default;
	WidgetEtat(const WidgetEtat &) = default;

	void render(void) override;
};
