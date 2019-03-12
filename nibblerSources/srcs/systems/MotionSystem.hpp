#pragma once

#include <cores/Univers.hpp>
#include <KINU/SystemsManager.hpp>

class MotionSystem : public KINU::System {
public:
	explicit MotionSystem(Univers &univers);
	MotionSystem() = delete;
	~MotionSystem() final;
	MotionSystem &operator=(const MotionSystem &) = delete;
	MotionSystem(const MotionSystem &) = delete;
	virtual void update();

private:
	Univers &univers_;
};
