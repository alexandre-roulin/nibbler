#pragma once

#include <cores/Univers.hpp>
#include <KINU/SystemsManager.hpp>

class JoystickSystem : public KINU::System {
public:
	explicit JoystickSystem();
	void update() override;
	virtual ~JoystickSystem();
};


