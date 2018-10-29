#pragma once


#include <KNU/systems/System.hpp>

class MotionSystem : public KNU::System {
public:
	explicit MotionSystem();
	void update() override;
	~MotionSystem() final;

};
