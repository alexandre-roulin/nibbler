#include "ComponentMask.hpp"


bool ComponentMask::operator==(ComponentMask const &componentMask) {
	return ((mask & componentMask.mask) == componentMask.mask);
}

template<typename C>
int ComponentMask::getComponentFamily() {
	return Component<typename std::remove_const<C>::type>::signature;
}

unsigned int ComponentMask::getMask() const {
	return mask;
}

template<typename ComponentType>
void ComponentMask::removeComponent() {
	mask ^= (1 << getComponentFamily<ComponentType>());
}

template<typename ComponentType>
void ComponentMask::addComponent() {
	mask |= (1 << getComponentFamily<ComponentType>());
}

ComponentMask::ComponentMask() : mask(0) {

}

void ComponentMask::clear() {
	mask = 0;
}
