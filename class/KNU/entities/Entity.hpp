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
#include <queue>
#include <logger.h>
#include <json/json.h>
#include <logger.h>
#include <logger_utils.h>
namespace KNU {

	class EntitiesManager;

	class World;

	class Entity {
	public:
		typedef int16_t ID;

	private:
		ID id;
		bool alive;

		Entity(ID id)
				: id(id),
				  alive(true) {
		};
		EntitiesManager *entityManager;

		EntitiesManager &getEntitiesManager() const;

		friend class EntitiesManager;

	public:
		Entity() : id(-1) {};

		Entity &operator=(Entity const &);

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
				   alive == rhs.alive;
		}

		bool operator!=(const Entity &rhs) const {
			return !(rhs == *this);
		}

		ID getId() const;

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

		Signature &getSignature();

		void tag(std::string tag);

		bool hasTag(std::string tag) const;

		void group(std::string group);

		bool hasGroup(std::string group) const;

		friend std::ostream &
		operator<<(std::ostream &os, const Entity &entity) {
			os << "id: " << entity.id << " " << " alive: "
			   << entity.alive;
			return os;
		}

	};

	class ComponentManager {
	private:
		std::vector<std::shared_ptr<AbstractPool>> componentPools;

		friend class EntitiesManager;

	public:
		ComponentManager();

		template<typename T>
		void addComponent(Entity &e, T component);

		template<typename T>
		T &getComponent(Entity const &entity) const;

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
		ComponentManager _componentManager;
		std::queue<Entity::ID> freeIds;
		std::vector<Entity> _entitiesMap;
		std::vector<Signature> _poolSignature;

		std::unordered_map<std::string, Entity> _taggedEntity;
		std::unordered_map<std::string, std::set<Entity>> _groupedEntities;
	public:
		EntitiesManager() = delete;

		explicit EntitiesManager(World &world);

		Entity &createEntity();

		Entity::ID getEntityId(Entity const &e) const;

		Entity getEntityById(Entity::ID id) const;

		void destroyEntity(Entity &entity);

		void tagEntity(Entity &e, std::string &tag);

		bool isEntityAlive(Entity const &entity) const;

		Entity getEntityByTag(std::string const &tag);

		bool hasTaggedEntity(std::string &tag);

		bool hasTag(std::string const &tag);

		bool hasGroup(std::string const &group);

		Json::Value serializeComponent() const;

		bool hasEntityInGroup(Entity const &entity, std::string group);

		void groupEntity(Entity &entity, std::string &group);

		std::vector<Entity> getEntitiesByGroup(std::string const &group);

		Signature &getEntitySignature(Entity &entity);

		unsigned long getGroupSize(std::string group);

		void killEntity(Entity &entity);

		template<typename T>
		T &getComponent(Entity const &entity) const;

		template<typename T>
		bool hasComponent(Entity const &entity) const;

		template<typename T>
		EntitiesManager &addComponent(Entity &entity, T component);

		template<typename T, typename ... Args>
		EntitiesManager &addComponent(Entity &entity, Args &&... args);

		template<typename T>
		void removeComponent(Entity &entity);

	};

	/** ENTITY **/

	template<typename T, typename... Args>
	void Entity::addComponent(Args &&... args) {
		getEntitiesManager().addComponent<T>(*this,
											 std::forward<Args>(args)...);
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
	T &EntitiesManager::getComponent(Entity const &entity) const {
		return _componentManager.getComponent<T>(entity);
	}

	/** COMPONENT MANAGER **/

	template<typename T>
	void ComponentManager::addComponent(Entity &entity, T component) {

		std::shared_ptr<Pool<T>> componentPool = accommodateComponent<T>();
		componentPool->add(entity, component);
	}

	template<typename T>
	T &ComponentManager::getComponent(Entity const &entity) const {
		const auto componentId = Component<T>::signature();
		auto componentPool =
				std::static_pointer_cast<Pool<T>>(componentPools[componentId]);
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
		if (componentPools[componentId] == nullptr) {
			return false;
		} else
			return std::static_pointer_cast<Pool<T>>
					(componentPools[componentId])->has(entity);
	}

	template<typename T>
	std::shared_ptr<Pool<T>> ComponentManager::accommodateComponent() {
		const auto componentId = Component<T>::signature();
		assert(componentId < COMPONENT_MAX);
		if (componentPools[componentId] == nullptr) {
			std::shared_ptr<Pool<T>> pool(new Pool<T>());
			componentPools[componentId] = pool;
		}
		return std::static_pointer_cast<Pool<T>>(componentPools[componentId]);
	}


	/** ENTITIES MANAGER **/



	template<typename T>
	bool EntitiesManager::hasComponent(Entity const &entity) const {
		return _componentManager.hasComponent<T>(entity);
	}

	template<typename T>
	EntitiesManager &
	EntitiesManager::addComponent(Entity &entity, T component) {
		_poolSignature[entity.id].addComponent<T>();
		_componentManager.addComponent(entity, component);
		return *this;
	}

	template<typename T, typename... Args>
	EntitiesManager &
	EntitiesManager::addComponent(Entity &entity, Args &&... args) {
		T component(std::forward<Args>(args) ...);
		addComponent<T>(entity, component);
		return *this;
	}

	template<typename T>
	void EntitiesManager::removeComponent(Entity &entity) {
//			entity.mask.removeComponent<T>();
		_componentManager.removeComponent<T>(entity);
	}


}



//E > [1]
//I > [1]