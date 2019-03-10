#pragma once


#include <KINU/SystemsManager.hpp>

class FollowSystem : public KINU::System {
public:
	FollowSystem();
	~FollowSystem() = default;
	FollowSystem &operator=(const FollowSystem &) = delete;
	FollowSystem(const FollowSystem &) = delete;

	virtual void update();
};


