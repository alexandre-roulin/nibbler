#pragma once


#include <KINU/System.hpp>

class MotionSystem : public KINU::System {
public:
	explicit MotionSystem();

	virtual void update();

	~MotionSystem() final;

};
