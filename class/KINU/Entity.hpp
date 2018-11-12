#pragma once

#include <cstdint>
#include <map>
#include <boost/type_index.hpp>
#include "AbstractPool.hpp"
#include "Pool.hpp"
#include "Component.hpp"
#include "Signature.hpp"
#include <unordered_map>
#include <unordered_set>
#include <logger.h>
#include <ostream>

#define BASE_ENTITY_CAPACITY 20

namespace KINU {

	/************** Entity **************/
	class World;

	class EntitiesManager;

	class Entity {
	public:
		typedef int16_t ID;

		operator bool();

		ID getId() const;

		Entity();

		Entity &operator=(const Entity &);

		Entity(const Entity &);

		bool isValid() const;

		void invalidate();

		/** Change state Entity **/

		void killGroup();

		void kill();

		/** Setter of entity's tag **/

		void tag(std::string tag);

		void group(std::string group);

		/** Getter of entity **/

		std::string &getTag();

		std::string &getGroup();

		Signature &getSignature();

		/** Checker of entity's tag **/

		bool hasTag();

		bool hasGroup();

		/** Component Manager **/

		template<typename T>
		void addComponent(T component);

		template<typename T, typename ... Args>
		void addComponent(Args &&... args);

		template<typename T>
		T &getComponent() const;

		template<typename T>
		bool hasComponent() const;

		template<typename T>
		void removeComponent();

		bool operator==(const Entity &rhs) const;

		bool operator!=(const Entity &rhs) const;

		bool operator<(const Entity &rhs) const;

		bool operator>(const Entity &rhs) const;

		bool operator<=(const Entity &rhs) const;

		bool operator>=(const Entity &rhs) const;

		EntitiesManager *entitiesManager;
	private:

		friend class EntitiesManager;
		ID id_;
		Signature signature;
		Entity(Entity::ID id);
	};


	/************** EntitiesManager **************/

	class EntitiesManager {
	private:

		//Reference to world
		World &world_;

		//Manage free id
		std::queue<Entity> freeIds;

		//all entities are here
		std::vector<Entity> entities;
		unsigned int size;
		unsigned int capacity;


		//Vector of pool of different component Pool<T> : AbstractPool
		std::vector<std::shared_ptr<AbstractPool>> componentPools;

		//Struct to manage Tag
		std::unordered_map<std::string, Entity> taggedEntities_;
		std::unordered_map<Entity::ID, std::string> tagId_;

		//Struct to manage Group
		std::unordered_map<std::string, std::set<Entity>> groupedEntities_;
		std::unordered_map<Entity::ID, std::string> groupId_;

	public:
		EntitiesManager(World &world);

		/** Creation && Supression Entity **/

		Entity createEntity();

		void deleteEntity(Entity &entity);

		/** Change state Entity **/

		void killEntity(Entity &entity);

		void killGroup(std::string group);

		/** Setter of entity's tag **/

		void tagEntity(Entity &entity, std::string tag);

		void groupEntity(Entity &entity, std::string group);

		/** Getter of entity **/

		Entity &getEntityByTag(std::string tag);

		Entity &getEntityById(Entity::ID id);

		std::vector<Entity> getEntitiesByGroup(std::string group);

		std::string &getTagOfEntity(Entity &entity);

		std::string &getGroupOfEntity(Entity &entity);

		Signature &getSignatureEntity(Entity &entity);

		Signature &getEntitySignature(Entity &entity);

		/** Checker of entity's tag **/

		bool hasTag(Entity &entity);

		bool hasGroup(Entity &entity);

		/** Component Manager **/

		template<typename T>
		void addComponent(Entity e, T component);

		template<typename T, typename ... Args>
		void addComponent(Entity e, Args &&... args);

		template<typename T>
		T &getComponent(Entity const &entity) const;

		template<typename T>
		std::shared_ptr<Pool<T>> accommodateComponent();

		template<typename T>
		bool hasComponent(Entity const &entity) const;

		template<typename T>
		void removeComponent(Entity &entity);
	};

	template<typename T>
	void Entity::addComponent(T component) {
		entitiesManager->addComponent<T>(*this, component);
	}

	template<typename T>
	T &Entity::getComponent() const {
		return entitiesManager->getComponent<T>(*this);
	}

	template<typename T>
	bool Entity::hasComponent() const {
		return entitiesManager->hasComponent<T>(*this);
	}

	template<typename T>
	void Entity::removeComponent() {
		entitiesManager->removeComponent<T>(*this);
	}

	template<typename T, typename... Args>
	void Entity::addComponent(Args &&... args) {
		entitiesManager->addComponent<T>(*this,
										 std::forward<Args>(args)...);
	}

	/************** EntitiesManager **************/

	template<typename T>
	void EntitiesManager::addComponent(Entity e, T component) {
//		log_trace("EntitiesManager::addComponent(%d, %s)", e.getId(), typeid(T).name());
		const auto entityId = e.getId();
		std::shared_ptr<Pool<T>> componentPool = accommodateComponent<T>();
		componentPool->set(entityId, component);
		e.signature.addComponent<T>();
	}

	template<typename T, typename ... Args>
	void EntitiesManager::addComponent(Entity e, Args &&... args) {
		T component(std::forward<Args>(args) ...);
		addComponent<T>(e, component);
	}

	template<typename T>
	T &EntitiesManager::getComponent(Entity const &entity) const {
		assert(entity.isValid());
		const auto componentId = Component<T>::signature();
		assert(componentPools[componentId] != nullptr);
		Entity::ID entityId = entity.getId();
		return std::static_pointer_cast<Pool<T>>(
				componentPools[componentId])->get(entityId);
	}

	template<typename T>
	bool EntitiesManager::hasComponent(Entity const &entity) const {
		assert(entity.isValid());
		const auto componentId = Component<T>::signature();
		Entity::ID entityId = entity.getId();

		if (componentPools[componentId] == nullptr)
			return false;
		else
			return std::static_pointer_cast<Pool<T>>(
					componentPools[componentId])->isValid(entityId);
	}

	template<typename T>
	void EntitiesManager::removeComponent(Entity &entity) {
		assert(entity.isValid());
		auto componentPool = accommodateComponent<T>();
		Entity::ID entityId = entity.getId();
		componentPool->invalidate(entityId);
		entity.signature.removeComponent<T>();
	}

	template<typename T>
	std::shared_ptr<Pool<T>> EntitiesManager::accommodateComponent() {

		const auto componentId = Component<T>::signature();

		if (componentId >= componentPools.size()) {
			componentPools.resize(componentId + 1, nullptr);
		}

		if (componentPools[componentId] == nullptr) {
			std::shared_ptr<Pool<T>> pool(new Pool<T>());
			componentPools[componentId] = pool;
		}

		return std::static_pointer_cast<Pool<T>>(componentPools[componentId]);
	}

}