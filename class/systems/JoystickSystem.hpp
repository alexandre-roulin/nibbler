#pragma once


#include <KINU/System.hpp>

class JoystickSystem : public KINU::System {
public:
	explicit JoystickSystem();
	void update() override;
	virtual ~JoystickSystem();
};


