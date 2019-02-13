#pragma once

#include "AWidget.hpp"
#include "nibbler.hpp"

class Gui;

class WidgetConnect : public AWidget {
public:
	WidgetConnect(Gui &);

	~WidgetConnect(void) override = default;


	void render(void) override;

private:
	bool client_;
	char dnsBuffer_[NAME_BUFFER];
	char portBuffer_[NAME_BUFFER];

	WidgetConnect &operator=(const WidgetConnect &) = delete;

	WidgetConnect(const WidgetConnect &) = delete;
};
