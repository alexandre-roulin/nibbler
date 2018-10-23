//
// Created by Alexandre ROULIN on 23/10/2018.
//

#ifndef NIBBLER_TRANSFORMSYSTEM_HPP
#define NIBBLER_TRANSFORMSYSTEM_HPP


#include "ASystem.hpp"

class TransformSystem : public ASystem {
public:
	TransformSystem();
	void update() final;

	virtual void registerEntity(Entity &e);

	virtual void unregisterEntity(Entity &e);

	~TransformSystem() final;
};


#endif //NIBBLER_TRANSFORMSYSTEM_HPP
