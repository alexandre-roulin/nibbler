
#include <iostream>
#include <KNU/component/TransformComponent.hpp>
#include <KNU/entities/Entity.hpp>
#include <KNU/managers/ComponentManager.hpp>

int main () {
	auto component = KNU::TransformComponent();
	auto entity = KNU::Entity();
	std::cout << KNU::TransformComponent::signature();

	auto componentManager = KNU::ComponentManager();

	componentManager.addComponent<KNU::TransformComponent>(entity, component);
}