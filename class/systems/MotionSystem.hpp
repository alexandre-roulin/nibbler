#pragma once


#include <KINU/System.hpp>

class MotionSystem : public KINU::System {
public:
	explicit MotionSystem();
	void update() override;
	~MotionSystem() final;

};
