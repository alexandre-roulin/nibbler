#pragma once
#include "AWidget.hpp"
#include "nibbler.hpp"

class Core;

class WidgetConnect : public AWidget {
public:
	WidgetConnect(Core &);
	~WidgetConnect(void);


	void	render(void);

private:
	bool			_client;
	char			_dnsBuffer[NAME_BUFFER];
	char			_portBuffer[NAME_BUFFER];

	WidgetConnect &operator=(const WidgetConnect&);
	WidgetConnect(const WidgetConnect&);
};
