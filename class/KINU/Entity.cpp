#include "Entity.hpp"
#include "World.hpp"
#include <iostream>

namespace KINU {

	/** *** ENTITY PART *** **/

	Entity::Entity() : id_(-1), entitiesManager_(nullptr) {

	}

	Entity::Entity(Entity::ID id) : id_(id) {

	}


	Entity::ID Entity::getId() const {
		return id_;
	}

	void Entity::kill() {
		getEntitiesManager_().killEntity(*this);
	}

	void Entity::killGroup() {
		getEntitiesManager_().killGroupEntity(*this);
	}

	void Entity::destroy() {
		getEntitiesManager_().destroyEntity(*this);
	}

	void Entity::tagByTagId(TagId tagId) {
		getEntitiesManager_().tagEntityByTagId(*this, tagId);
	}

	bool Entity::hasTagId() const {
		return getEntitiesManager_().hasTagIdByEntity(*this);
	}

	TagId Entity::getTagId() {
		return getEntitiesManager_().getTagIdByEntity(*this);
	}

	void Entity::groupEntityByGroupId(TagId tagId) {
		getEntitiesManager_().groupEntityByGroupId(*this, tagId);
	}

	bool Entity::hasGroupId() const {
		return getEntitiesManager_().hasGroupIdByEntity(*this);
	}

	TagId Entity::getGroupIdByEntity() {
		return getEntitiesManager_().getGroupIdByEntity(*this);
	}

	bool Entity::operator==(const Entity &rhs) const {
		return id_ == rhs.id_;
	}

	bool Entity::operator!=(const Entity &rhs) const {
		return !(rhs == *this);
	}

	EntitiesManager &Entity::getEntitiesManager_() const {
		assert(entitiesManager_ != nullptr);
		return *entitiesManager_;
	}

	void Entity::is(EntitiesManager &e) {
		entitiesManager_ = &e;
	}

	Entity Entity::operator=(Entity const &entity) {
		id_ = entity.id_;
		entitiesManager_ = entity.entitiesManager_;
		return *this;
	}


	/** *** ENTITIES_MANAGER PART *** **/

	EntitiesManager::EntitiesManager(World &world)
			: world_(world),
			  nextId(0) {

	}

	/** Constructor && Destructor entity **/

	Entity EntitiesManager::createEntity() {
		mutex_.lock();
		Entity::ID newInstance;

		if (freeIds.empty()) {
			newInstance = nextId++;
		} else {
			newInstance = freeIds.front();
			freeIds.pop_front();
		}
		Entity e(newInstance);
		e.entitiesManager_ = this;
		validId.push_back(newInstance);
		if (newInstance >= componentMasks.size())
			componentMasks.resize(newInstance + 1);
		mutex_.unlock();
		assert(!componentMasks[e.getId()].any());
		assert(!hasGroupIdByEntity(e));
		assert(!hasTagIdByEntity(e));
		assert(e.entitiesManager_ != nullptr);
		return e;
	}

	void EntitiesManager::destroyEntity(Entity entity) {
		// Reset componentMask of entity destroyed
		componentMasks[entity.id_].reset();

		mutex_.lock();
		validId.erase(std::remove_if(validId.begin(), validId.end(),
									 [entity](Entity::ID id) {
										 return entity.id_ == id;
									 }), validId.end());
		mutex_.unlock();
		// Remove groupId if exist

		if (hasGroupIdByEntity(entity)) {
			TagId groupId = getGroupIdByEntity(entity);
			mutex_.lock();
			groupedEntities[groupId].erase(std::remove_if(
					groupedEntities[groupId].begin(),
						   groupedEntities[groupId].end(),
						   [entity](auto entity1) {
							   return entity == entity1;
						   }));
			groupedEntityId.erase(entity.id_);
			mutex_.unlock();
		}
		// Remove tagId if exist

		if (hasTagIdByEntity(entity)) {
			TagId tagId = getTagIdByEntity(entity);
			mutex_.lock();
			taggedEntityId.erase(entity.id_);
			taggedEntities.erase(tagId);
			mutex_.unlock();
		}
	}


	ComponentMask &EntitiesManager::getComponentMask(Entity entity) {

		return componentMasks[entity.getId()];
	}

	/** Kill Management **/

	void EntitiesManager::killEntity(Entity entity) {
		validId.erase(std::remove_if(validId.begin(), validId.end(), [entity](Entity::ID id){ return entity.getId() == id;}));
		world_.destroyEntity(entity);
	}

	void EntitiesManager::killGroupEntity(Entity entity) {
		if (hasGroupIdByEntity(entity)) {
			std::vector<Entity> entities = getEntitiesByGroupId(getGroupIdByEntity(entity));
			for (auto entity_ : entities) {
				killEntity(entity_);
			}
		}
	}

	bool EntitiesManager::hasEntityById(Entity::ID id) {
		mutex_.lock();
		bool has = std::find(validId.begin(), validId.end(), id) != validId.end();
		mutex_.unlock();
		return has;
	}

	Entity EntitiesManager::getEntityById(Entity::ID id) {
		assert(hasEntityById(id));
		Entity entity(id);
		entity.entitiesManager_ = this;
		return entity;
	}

	/** TAG FUNCTION **/

	bool EntitiesManager::hasTagIdByEntity(Entity entity) {
		mutex_.lock();
		bool tag = taggedEntityId.find(entity.id_) != taggedEntityId.end();
		mutex_.unlock();
		bool has = hasEntityById(entity.getId());
		return tag && has;
	}

	TagId EntitiesManager::getTagIdByEntity(Entity entity) {
		assert(hasTagIdByEntity(entity));
		mutex_.lock();
		TagId tag = taggedEntityId[entity.id_];
		mutex_.unlock();
		return tag;
	}

	bool EntitiesManager::hasEntityByTagId(TagId tagId) {
		mutex_.lock();
		auto it = taggedEntities.find(tagId);
		bool tagged = it != taggedEntities.end();
		mutex_.unlock();
		bool has = hasEntityById(it->second.getId());
		return tagged && has;
	}

	Entity EntitiesManager::getEntityByTagId(TagId tagId) {
		assert(hasEntityByTagId(tagId));
		mutex_.lock();
		Entity e = taggedEntities[tagId];
		e.entitiesManager_ = this;
		mutex_.unlock();
		return e;
	}

	void EntitiesManager::tagEntityByTagId(Entity entity,
										   TagId tagId) {
		taggedEntities[tagId] = entity;
		taggedEntityId[entity.id_] = tagId;
	}

	/** GROUP FUNCTION */

	bool
	EntitiesManager::hasEntitiesGroupId(TagId tagId) {
		mutex_.lock();
		auto it = groupedEntities.find(tagId);
		bool has = it != groupedEntities.end() && std::any_of(it->second.begin(), it->second.end(), [this](Entity entity){
			mutex_.unlock();
			bool has = hasEntityById(entity.getId());
			mutex_.lock();
			return has;
		}
		);
		mutex_.unlock();
		return has;
	}

	std::vector<Entity>
	EntitiesManager::getEntitiesByGroupId(TagId tagId) {
		assert(hasEntitiesGroupId(tagId));
		mutex_.lock();
		std::vector<Entity> entities = groupedEntities[tagId];
		mutex_.unlock();
		return entities;
	}

	void EntitiesManager::groupEntityByGroupId(Entity entity,
											   TagId tagId) {
		mutex_.lock();
		groupedEntityId[entity.id_] = tagId;
		groupedEntities[tagId].push_back(entity);
		mutex_.unlock();
	}

	bool EntitiesManager::hasGroupIdByEntity(Entity entity) {
		mutex_.lock();
		bool grp = groupedEntityId.find(entity.id_) != groupedEntityId.end();
		mutex_.unlock();
		bool has = hasEntityById(entity.getId());
		return grp && has;
	}

	TagId EntitiesManager::getGroupIdByEntity(Entity entity) {
		assert(hasGroupIdByEntity(entity));
		mutex_.lock();
		TagId tag = groupedEntityId[entity.id_];
		mutex_.unlock();
		return tag;
	}

}