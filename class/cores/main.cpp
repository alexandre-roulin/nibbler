
#include <iostream>
#include <KNU/component/TransformComponent.hpp>
#include <KNU/entities/Entity.hpp>
#include <KNU/managers/ComponentManager.hpp>
#include <KNU/component/Component.hpp>

int main() {
	auto component = KNU::TransformComponent(42, 42);

	KNU::Entity entity = KNU::Entity();
	std::cout << "Signature : "
			  << KNU::Component<KNU::TransformComponent>::signature()
			  << std::endl;
	std::cout << component.x << std::endl;
	auto componentManager = KNU::ComponentManager();

	componentManager.addComponent(entity, component);
	auto &componentFromComponentManager = componentManager.getComponent<KNU::TransformComponent>(entity);
	std::cout << componentFromComponentManager.x << std::endl;
//	std::cout << componentFromComponentManager << std::endl;

}