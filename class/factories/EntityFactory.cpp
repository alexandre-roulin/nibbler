//
// Created by Alexandre ROULIN on 23/10/2018.
//

#include <ECS.hpp>
#include "EntityFactory.hpp"
#include <managers/ComponentManager.hpp>
#include <managers/EntitiesManager.hpp>
#include <managers/SystemManager.hpp>

template<typename ComponentType>
EntityFactory &EntityFactory::addComponent(const ComponentType &c) {
	ecs.getComponentManager()->addComponent(e, c);
	e.mask.addComponent<ComponentType>();
	return *this;
}

EntityFactory &EntityFactory::createEntity(std::string tag) {
	e = ecs.getEntitiesManager()->createEntity(tag);
	return *this;
}



EntityFactory::EntityFactory(ECS const &ecs) : ecs(ecs) {

}

Entity &EntityFactory::build() {
	return e;
}
