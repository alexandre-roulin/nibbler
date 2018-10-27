#pragma once


#include <KNU/systems/System.hpp>

class JoystickSystem : public KNU::System {
public:
	explicit JoystickSystem();
	void update() override;
	virtual ~JoystickSystem();
};


