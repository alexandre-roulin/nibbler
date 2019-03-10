#pragma once

#include <cores/Univers.hpp>
#include <KINU/SystemsManager.hpp>

class JoystickSystem : public KINU::System {
public:
	explicit JoystickSystem();
	virtual ~JoystickSystem();
	JoystickSystem &operator=(const JoystickSystem &) = delete;
	JoystickSystem(const JoystickSystem &) = delete;

	void update() override;
};


