
//				ECS
//					-> Entity
//						-> addComponent(...)
//							->addSystems<MotionSystem>(Entity { MOTION_COMPONENT { velocity | speed  } | TRANSFORM_COMPONENT { x , y } } ) 					(Move Sprite)
//
//
//


#include <components/MotionComponent.hpp>
#include <components/TransformComponent.hpp>
#include <systems/MotionSystem.hpp>
#include "ECS.hpp"
#include <managers/SystemManager.hpp>
#include <managers/EntitiesManager.hpp>

int main() {
//	ECS ecs = ECS();

	/*
	 * 	Creation System with MotionComponent | TransformComponent signature
	 * 	And add to system manager
	 */
//	MotionSystem *ms = new MotionSystem();
//	ms->addSignature<MotionComponent>();
//	ms->addSignature<TransformComponent>();
//	ecs.getSystemManager()->addToSystems(ms);

	/*
	 * Create Entity Snake head
	 * add Component Motion and Transform
	 */

//	Entity snake_head = ecs.getEntitiesManager()->getEntityFactory()->createEntity("snake")
//			.addComponent(MotionComponent())
//			.addComponent(TransformComponent())
//			.build();

	return 0;
}