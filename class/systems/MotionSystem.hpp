#pragma once


#include <KNU/systems/System.hpp>

class MotionSystem : public KNU::System {
public:
	void update() override;
	explicit MotionSystem();
	~MotionSystem() final;

};
