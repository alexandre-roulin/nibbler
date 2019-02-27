#pragma once

#include "AWidget.hpp"
#include "nibbler.hpp"

class Gui;

class WidgetOption : public AWidget {
public:
	WidgetOption(Gui &);
	~WidgetOption() override = default;
	WidgetOption() = delete;
	WidgetOption &operator=(const WidgetOption &) = delete;
	WidgetOption(const WidgetOption &) = delete;

	void render(void) override;

private:
	unsigned int mapSize_;
	char nameBuffer_[NAME_BUFFER];
	bool sound_;
};
