#pragma once


#include <KINU/System.h>

class MotionSystem : public Mix::System {
public:
	explicit MotionSystem();

	virtual void update();

	~MotionSystem() final;

};
