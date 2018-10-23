//
// Created by Alexandre ROULIN on 23/10/2018.
//

#ifndef NIBBLER_SYSTEMFACTORY_HPP
#define NIBBLER_SYSTEMFACTORY_HPP


#include <systems/ASystem.hpp>
#include <ECS.hpp>

class SystemFactory {
private:
	ASystem *sys;
	ECS const &ecs;
public:
	explicit SystemFactory(ECS &ecs);

	SystemFactory &initSystem(ASystem &);

	template <typename ComponentType>
	SystemFactory &addSignature();
};



#endif //NIBBLER_SYSTEMFACTORY_HPP
