#pragma once


#include <KNU/systems/System.hpp>

class MotionSystem : public KNU::System {
public:
	virtual void update();

	~MotionSystem() final;

};
