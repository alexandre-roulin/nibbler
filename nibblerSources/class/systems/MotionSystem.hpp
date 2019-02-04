#pragma once

#include <Univers.hpp>
#include <KINU/SystemsManager.hpp>

class MotionSystem : public KINU::System {
public:
	explicit MotionSystem(Univers &univers);

	virtual void update();

	~MotionSystem() final;

private:
	Univers &univers_;
};
