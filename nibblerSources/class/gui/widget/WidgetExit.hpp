#pragma once

#include "AWidget.hpp"

class WidgetExit : public AWidget {
public:
	WidgetExit(Gui &, std::function<void(Gui&)> callbackExit);

	~WidgetExit(void) override = default;


	void render(void) override;

private:
	std::function<void(Gui&)> callbackExit_;

	WidgetExit &operator=(const WidgetExit &) = delete;

	WidgetExit(const WidgetExit &) = delete;

	WidgetExit(void) = delete;
};
