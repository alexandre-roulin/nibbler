#pragma once

#include "AWidget.hpp"
#include "nibbler.hpp"

class Gui;

class WidgetServerPannel : public AWidget {
public:
	WidgetServerPannel(Gui &);
	~WidgetServerPannel() override = default;
	WidgetServerPannel() = delete;
	WidgetServerPannel &operator=(const WidgetServerPannel &) = delete;
	WidgetServerPannel(const WidgetServerPannel &) = delete;

private:
	void content_(bool renderContentInWindow) override;
};
