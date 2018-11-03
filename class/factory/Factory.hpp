#pragma once


#include <nibbler.hpp>
#include <KNU/World.hpp>
#include <KNU/entities/Entity.hpp>
#include <component/CollisionComponent.hpp>
#include <component/JoystickComponent.hpp>
#include <component/MotionComponent.hpp>
#include <component/PositionComponent.hpp>
#include <component/FollowComponent.hpp>

class Factory {
public:
	Factory(KNU::World &world);
	void create_all_snake(int16_t nu);
	void create_snake(int16_t , int, int);
	void create_food(int, int);
private:
	KNU::World &world_;
};


