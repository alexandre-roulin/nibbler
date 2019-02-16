#pragma once

#include "AWidget.hpp"
#include "nibbler.hpp"

class Gui;

class WidgetConnect : public AWidget {
public:
	WidgetConnect(Gui &);
	~WidgetConnect() override = default;
	WidgetConnect() = delete;
	WidgetConnect &operator=(const WidgetConnect &) = default;
	WidgetConnect(const WidgetConnect &) = default;

	void render(void) override;

private:
	bool client_;
	char dnsBuffer_[NAME_BUFFER];
	char portBuffer_[NAME_BUFFER];

};
