
#include <iostream>
#include <KNU/component/TransformComponent.hpp>
#include <KNU/entities/Entity.hpp>
#include <KNU/managers/ComponentManager.hpp>
#include <KNU/component/Component.hpp>
int main () {
	auto component = KNU::TransformComponent();
	KNU::Entity entity = KNU::Entity();
	std::cout << KNU::Component<KNU::TransformComponent>::signature();

	auto componentManager = KNU::ComponentManager();

	componentManager.addComponent(entity, component);
}