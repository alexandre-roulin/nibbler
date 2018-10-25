
#include "Entity.hpp"
#include <KNU/managers/EntitiesManager.hpp>

namespace KNU {

	const Signature &Entity::getMask() {
		return mask;
	}

	void Entity::kill() {
		getEntitiesManager().killEntity(*this);
	}

	bool Entity::isAlive() const {
		return getEntitiesManager().isEntityAlive(*this);
	}
	EntitiesManager &Entity::getEntitiesManager() const {
		return *entityManager;
	}



	void Entity::tag(std::string tag) {
		getEntitiesManager().tagEntity(*this, tag);
	}

	bool Entity::hasTag(std::string tag) const {
		return getEntitiesManager().hasTaggedEntity(tag);
	}

	void Entity::group(std::string group) {

	}

	bool Entity::hasGroup(std::string group) const {
		return false;
	}
	template<typename T>
	void Entity::addComponent(T component) {
		getEntitiesManager().addComponent(*this, component);
	}

	template<typename T, typename... Args>
	void Entity::addComponent(Args &&... args) {
		getEntitiesManager().addComponent(*this, std::forward<Args>(args)...);
	}

	template<typename T>
	void Entity::removeComponent() {
		getEntitiesManager().removeComponent<T>(*this);
	}

	template<typename T>
	bool Entity::hasComponent() const {
		return getEntitiesManager().hasComponent<T>(*this);
	}

	template<typename T>
	T &Entity::getComponent() const {
		return getEntitiesManager().getComponent<T>(*this);
	}
}