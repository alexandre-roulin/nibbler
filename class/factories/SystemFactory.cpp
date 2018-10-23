//
// Created by Alexandre ROULIN on 23/10/2018.
//

#include "SystemFactory.hpp"


SystemFactory::SystemFactory(ECS &ecs) : ecs(ecs) {

}

template<typename ComponentType>
SystemFactory &SystemFactory::addSignature() {
	assert(sys != nullptr);
	(*sys).cm.addComponent<ComponentType>();
	return *this;
}

SystemFactory &SystemFactory::initSystem(ASystem &system) {
	sys = &system;
	return *this;
}

