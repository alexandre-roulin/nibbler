//
// Created by Alexandre ROULIN on 22/10/2018.
//

#include <components/MotionComponent.hpp>
#include "ComponentManager.hpp"

ComponentManager::ComponentManager(ECS const &ecs) : ecs(ecs) {

}

template<typename ComponentType>
void ComponentManager::addComponent(Entity e, ComponentType c) {

	auto &pool = _pools[typeid(ComponentType)];
	if (pool.size() < e.id)
		pool.resize(pool.size() + e.id);
	pool.set(e.id, c);
}

template<typename ComponentType>
ComponentType ComponentManager::getComponent(Entity e) {
	return _pools[typeid(ComponentType)].get(e.id);
}


//					CHAR
//				/		  \
//			WAR				WIZARD


//HP - SWORD - SPELL


//WAR { HP - SWORD }					WIZ { HP - SPELL }
// HPMANAGER { HP(WAR), HP(WIZ  100 )HP(WAR), HP(WIZ)HP(WAR), HP(WIZ)HP(WAR), HP(WIZ)HP(WAR), HP(WIZ)HP(WAR), HP(WIZ)HP(WAR), HP(WIZ)HP(WAR), HP(WIZ)HP(WAR), HP(WIZ)HP(WAR), HP(WIZ)HP(WAR), HP(WIZ)HP(WAR), HP(WIZ)}
// SWORDMANAGER { SWORD(WAR 10 ) }
// SPELL MANAGER { SPELL(WIZ) }