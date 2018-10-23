//
// Created by Alexandre ROULIN on 23/10/2018.
//

#ifndef NIBBLER_MOTIONSYSTEM_HPP
#define NIBBLER_MOTIONSYSTEM_HPP


#include <entity/Entity.hpp>
#include <vector>
#include "ASystem.hpp"
#include <map>

class MotionSystem : public ASystem {
private:
	ComponentMask cm;
public:
	virtual void registerEntity(Entity &e);

	virtual void unregisterEntity(Entity &e);

	template <typename ComponentType>
	void addSignature();

	template <typename ComponentType>
	void removeSignature();

	void update() final;

	~MotionSystem() final;
};


#endif //NIBBLER_MOTIONSYSTEM_HPP
