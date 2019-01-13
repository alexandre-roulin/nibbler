#pragma once

#include <cstdint>
#include <deque>
#include <vector>
#include "Component.hpp"
#include "Pool.hpp"
#include <map>
#include <unordered_map>
#include <logger.h>
#include <mutex>

namespace KINU {

	class World;

	class EntitiesManager;

	using TagId = int16_t;

	class Entity {
	public:
		using ID = uint16_t;

		Entity();

		bool operator==(const Entity &rhs) const;

		bool operator!=(const Entity &rhs) const;

		ID getId() const;

		/** Kill Management**/

		void kill();

		void killGroup();

		/** Tag Management **/

		// Tag entity with tag id
		void tagByTagId(TagId);

		// Check if entity has any tag id
		bool hasTagId() const;

		// Getter tag id by entity
		TagId getTagId();


		/** Group Management **/

		// Group entity by tag id
		void groupEntityByGroupId(TagId);

		// Check if entity has any group id
		bool hasGroupId() const;

		// get group id of possible group entity
		TagId getGroupIdByEntity();

		/** Component Management **/

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

	private:
		friend class EntitiesManager;

		Entity(Entity::ID);

	public:
		EntitiesManager &getEntitiesManager_() const;

	private:

		Entity::ID id_;
		EntitiesManager *entitiesManager_;
	};

	class EntitiesManager {
	public:
		EntitiesManager(World &world);


		/** Constructor && Destructor**/
		Entity createEntity();

		void destroyEntity(Entity);

		/** ComponentMask **/

		ComponentMask &getComponentMask(Entity);

		/** Kill Management**/

		void killEntity(Entity);

		void killGroupEntity(Entity);

		/** ID Management **/

		bool hasEntityById(Entity::ID);

		Entity getEntityById(Entity::ID);

		/** Tag Management **/

		// Getter entity by tag id
		Entity getEntityByTagId(TagId);

		// Check if there is any entity tag by tag id
		bool hasEntityByTagId(TagId) const;

		// Tag entity with tag id
		void tagEntityByTagId(Entity, TagId);

		// Check if entity has any tag id
		bool hasTagIdByEntity(Entity) const;

		// Getter tag id by entity
		TagId getTagIdByEntity(Entity);

		/** Group Management **/

		// Getter group entities by tag id
		std::vector<Entity>
		getEntitiesByGroupId(TagId);

		// Check if there is any group with tag id
		bool hasEntitiesGroupId(TagId) const;

		// Group entity by tag id
		void groupEntityByGroupId(Entity, TagId);

		// Check if entity has any group id
		bool hasGroupIdByEntity(Entity) const;

		// get group id of possible group entity
		TagId getGroupIdByEntity(Entity);

		/** Component Management **/

		template<typename T>
		void addComponent(Entity, T component);

		template<typename T, typename ... Args>
		void addComponent(Entity e, Args &&... args);

		template<typename T>
		void removeComponent(Entity);

		template<typename T>
		bool hasComponent(Entity) const;

		template<typename T>
		T &getComponent(Entity) const;

		template<typename T>
		std::shared_ptr<Pool<T>> accommodateComponent();

	private:
		//Group variable
		std::unordered_map<Entity::ID, TagId> groupedEntityId; // Entity::ID = PlayerID
		std::unordered_map<TagId, std::vector<Entity>> groupedEntities; // [PlayerID] = std::vector<Entity::ID>{0, 1, 2, ...}

		//Tag variable
		std::unordered_map<Entity::ID, TagId> taggedEntityId; // Entity::ID = PlayerID
		std::unordered_map<TagId, Entity> taggedEntities; // [PlayerID] = Entity::ID

		// Id variable
		Entity::ID nextId; // next slot available
		std::deque<Entity::ID> freeIds; // freeIds slots

		// ComponentMask Pool
		std::vector<ComponentMask> componentMasks; // [Entity::ID] = Mask

		// Component Pool
		std::vector<std::shared_ptr<AbstractPool>> componentPools;

		// Entity pool
		std::vector<Entity::ID> validId;

		World &world_;

		std::mutex mutex_;
	};

	/** Component Management from Entity **/

	template<typename T>
	void Entity::addComponent(T component) {
		getEntitiesManager_().addComponent<T>(*this, component);
	}

	template<typename T, typename... Args>
	void Entity::addComponent(Args &&... args) {
		getEntitiesManager_().addComponent<T>(*this, std::forward<Args>(args)...);
	}

	template<typename T>
	void Entity::removeComponent() {
		getEntitiesManager_().removeComponent<T>(*this);
	}

	template<typename T>
	bool Entity::hasComponent() const {
		return getEntitiesManager_().hasComponent<T>(*this);
	}

	template<typename T>
	T &Entity::getComponent() const {
		return getEntitiesManager_().getComponent<T>(*this);
	}
	/** Component Management from EntitiesManager **/

	template<typename T>
	void EntitiesManager::addComponent(Entity entity, T component) {
		const auto componentId = Component<T>::getId();
		const auto entityId = entity.getId();
		std::shared_ptr<Pool<T>> componentPool = accommodateComponent<T>();

		if (entityId >= componentPool->getSize()) {
			componentPool->resize(entityId + DEFAULT_POOL_SIZE_COMPONENT);
		}

		componentPool->set(entityId, component);
		componentMasks[entityId].set(componentId);
	}

	template<typename T, typename... Args>
	void EntitiesManager::addComponent(Entity entity, Args &&... args) {
		T component(std::forward<Args>(args) ...);
		addComponent<T>(entity, component);

	}


	template<typename T>
	void EntitiesManager::removeComponent(Entity entity) {
		const auto componentId = Component<T>::getId();
		const auto entityId = entity.getId();
		assert(entityId < componentMasks.size());
		componentMasks[entityId].set(componentId, false);

	}

	template<typename T>
	bool EntitiesManager::hasComponent(Entity entity) const {
		const auto componentId = Component<T>::getId();
		const auto entityId = entity.getId();
		assert(entityId < componentMasks.size());
		return componentMasks[entityId].test(componentId);
	}

	template<typename T>
	T &EntitiesManager::getComponent(Entity entity) const {
		const auto componentId = Component<T>::getId();
		const auto entityId = entity.getId();

		assert(hasComponent<T>(entity));
		assert(componentId < componentPools.size());
		auto componentPool = std::static_pointer_cast<Pool<T>>(
				componentPools[componentId]);

		assert(componentPool);
		assert(entityId < componentPool->getSize());
		return componentPool->get(entityId);
	}

	template<typename T>
	std::shared_ptr<Pool<T>> EntitiesManager::accommodateComponent() {
		auto const componentId = Component<T>::getId();

		if (componentId >= componentPools.size()) {
			componentPools.resize(componentId + 1, nullptr);
		}

		if (!componentPools[componentId]) {
			std::shared_ptr<Pool<T>> pool(new Pool<T>(DEFAULT_POOL_SIZE_COMPONENT));
			componentPools[componentId] = pool;
		}

		return std::static_pointer_cast<Pool<T>>(
				componentPools[componentId]);
	}



}
