#pragma once

#include <map>
#include <string>
#include <ostream>
#include <KNU/utils/Signature.hpp>
#include <KNU/managers/AbstractPool.hpp>
#include <KNU/component/Component.hpp>
#include <nibbler.hpp>
#include <vector>
#include <KNU/managers/Pool.hpp>
#include <unordered_map>
#include <set>

namespace KNU {
	class EntitiesManager;

	class World;

	class Entity {
	private:
		int id;
		Signature mask;
		bool alive;

		Entity(unsigned int id)
				: id(id),
				  alive(true) {
		};
		EntitiesManager *entityManager;

		EntitiesManager &getEntitiesManager() const;

		friend class EntitiesManager;

	public:
		Entity() : id(-1) {};


		Signature const &getMask();

		bool operator<(const Entity &rhs) const {
			return id < rhs.id;
		}

		bool operator>(const Entity &rhs) const {
			return rhs < *this;
		}

		bool operator<=(const Entity &rhs) const {
			return !(rhs < *this);
		}

		bool operator>=(const Entity &rhs) const {
			return !(*this < rhs);
		}

		bool operator==(const Entity &rhs) const {
			return id == rhs.id &&
				   mask == rhs.mask &&
				   alive == rhs.alive;
		}

		bool operator!=(const Entity &rhs) const {
			return !(rhs == *this);
		}


		void kill();

		bool isAlive() const;

		template<typename T>
		void addComponent(T component);

		template<typename T, typename ... Args>
		void addComponent(Args &&... args);

		template<typename T>
		void removeComponent();

		template<typename T>
		bool hasComponent() const;

		template<typename T>
		T &getComponent() const;

		void tag(std::string tag);

		bool hasTag(std::string tag) const;

		void group(std::string group);

		bool hasGroup(std::string group) const;

		friend std::ostream &
		operator<<(std::ostream &os, const Entity &entity) {
			os << "id: " << entity.id << " " << entity.mask << " alive: "
			   << entity.alive;
			return os;
		}
	};

	class ComponentManager {
	private:
		std::vector<std::shared_ptr<AbstractPool>> componentPools;

	public:
		ComponentManager() {
			componentPools.resize(COMPONENT_MAX);
		}

		ComponentManager(ComponentManager const &) {

		}

		template<typename T>
		void addComponent(Entity &e, T component);

		template<typename T>
		T &getComponent(Entity &entity) const;

		template<typename T>
		void removeComponent(Entity &entity);

		template<typename T>
		bool hasComponent(Entity const &entity) const;

		template<typename T>
		std::shared_ptr<Pool<T>> accommodateComponent();


	};


	class EntitiesManager {
	private:
		unsigned int size;
		unsigned int capacity;
		World &world;
		ComponentManager &_componentManager;
		std::vector<Entity> _entitiesMap;
		std::unordered_map<std::string, Entity> _taggedEntity;
		std::unordered_map<std::string, std::set<Entity>> _groupedEntities;
	public:
		EntitiesManager() = delete;

		explicit EntitiesManager(World &world, ComponentManager &componentManager);

		Entity &createEntity();

		void destroyEntity(Entity &entity);

		void tagEntity(Entity &e, std::string &tag);

		bool isEntityAlive(Entity e) const;

		Entity getEntityByTag(std::string const &tag);

		bool hasTaggedEntity(std::string &tag);

		bool hasTag(std::string const &tag);

		bool hasGroup(std::string const &group);

		void groupEntity(Entity &e, std::string &group);

		std::vector<Entity> getEntitiesByGroup(std::string const &group);

		unsigned long getGroupSize(std::string group);

		void killEntity(Entity &e);

		template<typename T>
		T &getComponent(Entity &e) const;

		template<typename T>
		bool hasComponent(Entity const &e) const;

		template<typename T>
		EntitiesManager &addComponent(Entity &e, T component);

		template<typename T, typename ... Args>
		EntitiesManager &addComponent(Entity &e, Args &&... args);

		template<typename T>
		void removeComponent(Entity &e);

	};

	/** ENTITY **/

	template<typename T, typename... Args>
	void Entity::addComponent(Args &&... args) {
		getEntitiesManager().addComponent<T>(*this, std::forward<Args>(args)...);
	}

	template<typename T>
	void Entity::removeComponent() {
		getEntitiesManager().removeComponent<T>(*this);
	}


	template<typename T>
	T &Entity::getComponent() const {
		return getEntitiesManager().getComponent<T>(*this);
	}

	template<typename T>
	void Entity::addComponent(T component) {
		getEntitiesManager().addComponent(*this, component);
	}

	template<typename T>
	bool Entity::hasComponent() const {
		return getEntitiesManager().hasComponent<T>(*this);
	}

	template<typename T>
	T &EntitiesManager::getComponent(Entity &e) const {
		assert(e.mask.getMask() & Component<T>::signature());
		return _componentManager.getComponent<T>(e);
	}

	/** COMPONENT MANAGER **/

	template<typename T>
	void ComponentManager::addComponent(Entity &e, T component) {
		const auto componentId = Component<T>::signature();
		std::shared_ptr<Pool<T>>
				componentPool = accommodateComponent<T>();
		componentPool->add(e, component);
	}

	template<typename T>
	T &ComponentManager::getComponent(Entity &entity) const {
		std::shared_ptr<Pool<T>>
				componentPool = accommodateComponent<T>();
		return componentPool->get(entity);

	}

	template<typename T>
	void ComponentManager::removeComponent(Entity &entity) {

		std::shared_ptr<Pool<T>>
				componentPool = accommodateComponent<T>();
		componentPool->remove(entity);
	}

	template<typename T>
	bool ComponentManager::hasComponent(Entity const &entity) const {
		const auto componentId = Component<T>::signature();
		auto componentPool = std::static_pointer_cast<Pool<T>>(componentPools[componentId]);
		return componentPool->has(entity);
	}

	template<typename T>
	std::shared_ptr<Pool<T>> ComponentManager::accommodateComponent() {
		const auto componentId = Component<T>::signature();
		assert(componentId < COMPONENT_MAX);
		if (!componentPools[componentId]) {
			std::shared_ptr<Pool<T>> pool(new Pool<T>());
			componentPools[componentId] = pool;
		}
		return std::static_pointer_cast<Pool<T>>(componentPools[componentId]);

	}

	/** ENTITIES MANAGER **/

	template<typename T>
	bool EntitiesManager::hasComponent(Entity const &e) const {
		return _componentManager.hasComponent<T>(e);
	}

	template<typename T>
	EntitiesManager &EntitiesManager::addComponent(Entity &e, T component) {
		_componentManager.addComponent(e, component);
		return *this;
	}

	template<typename T, typename... Args>
	EntitiesManager &EntitiesManager::addComponent(Entity &e, Args &&... args) {
		T component(std::forward<Args>(args) ...);
		addComponent<T>(e, component);
		return *this;
	}

	template<typename T>
	void EntitiesManager::removeComponent(Entity &e) {
//			e.mask.removeComponent<T>();
		_componentManager.removeComponent<T>(e);
	}
}