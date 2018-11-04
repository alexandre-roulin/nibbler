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

	WidgetOption &operator=(const WidgetOption&);
	WidgetOption(const WidgetOption&);
};
