#pragma once
#include "AWidget.hpp"
#include "nibbler.hpp"

class Core;

class WidgetOption : public AWidget {
public:
	WidgetOption(Core &);
	~WidgetOption(void);


	void	render(void);

private:
	bool	_client;
	char	_nameBuffer[NAME_BUFFER];
	char	_dnsBuffer[NAME_BUFFER];
	char	_portBuffer[NAME_BUFFER];

	WidgetOption &operator=(const WidgetOption&);
	WidgetOption(const WidgetOption&);
};
