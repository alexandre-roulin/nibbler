#pragma once


#include <KINU/SystemsManager.hpp>

class MotionSystem : public KINU::System {
public:
	explicit MotionSystem();

	virtual void update();

	~MotionSystem() final;

};
