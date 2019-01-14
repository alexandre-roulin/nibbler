#pragma once
#include "AWidget.hpp"
#include <string>
#include <SFML/Graphics.hpp>
#include "WidgetSnake.hpp"
#include "nibbler.hpp"

class Core;

class WidgetMassiveButton : public AWidget {
public:
	WidgetMassiveButton(Core &core);
	~WidgetMassiveButton(void);

	void	render(void);

private:

	WidgetMassiveButton &operator=(const WidgetMassiveButton&);
	WidgetMassiveButton(const WidgetMassiveButton&);
};
