#pragma once

#include "AWidget.hpp"

class WidgetExit : public AWidget {
public:
	WidgetExit(Gui &, std::function<void(Gui&)> callbackExit);
	~WidgetExit() override = default;
	WidgetExit() = delete;
	WidgetExit &operator=(const WidgetExit &) = default;
	WidgetExit(const WidgetExit &) = default;

	void render(void) override;

private:
	std::function<void(Gui&)> callbackExit_;

};
