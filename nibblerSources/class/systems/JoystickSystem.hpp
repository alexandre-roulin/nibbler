#pragma once


#include <KINU/SystemsManager.hpp>

class JoystickSystem : public KINU::System {
public:
	explicit JoystickSystem(Univers &univers);
	void update() override;

	virtual ~JoystickSystem();
private:
	Univers &univers_;
};


