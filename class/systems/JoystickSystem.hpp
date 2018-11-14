#pragma once


#include <KINU/System.h>

class JoystickSystem : public Mix::System {
public:
	explicit JoystickSystem();
	void update() override;
	virtual ~JoystickSystem();
};


