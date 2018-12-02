#include "Entity.hpp"
#include "World.hpp"

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


	/** *** ENTITIES_MANAGER PART *** **/

	EntitiesManager::EntitiesManager(World &world)
			: world_(world),
			  nextId(0) {

	}

	/** Constructor && Destructor entity **/

	Entity EntitiesManager::createEntity() {

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
		assert(!componentMasks[e.getId()].any());
		assert(!hasGroupIdByEntity(e));
		assert(!hasTagIdByEntity(e));
		return e;
	}

	void EntitiesManager::destroyEntity(Entity entity) {

		// Reset componentMask of entity destroyed
		componentMasks[entity.id_].reset();

		validId.erase(std::remove_if(validId.begin(), validId.end(),
									 [entity](Entity::ID id) {
										 return entity.id_ == id;
									 }));
		// Remove groupId if exist

		if (hasGroupIdByEntity(entity)) {
			TagId groupId = getGroupIdByEntity(entity);
			std::remove_if(groupedEntities[groupId].begin(),
						   groupedEntities[groupId].end(),
						   [entity](auto const entity1) {
							   return entity == entity1;
						   });
			groupedEntityId.erase(entity.id_);
		}
		// Remove tagId if exist

		if (hasTagIdByEntity(entity)) {
			TagId tagId = getTagIdByEntity(entity);
			taggedEntityId.erase(entity.id_);
			taggedEntities.erase(tagId);
		}
	}


	ComponentMask &EntitiesManager::getComponentMask(Entity entity) {
		return componentMasks[entity.getId()];
	}

	/** Kill Management **/

	void EntitiesManager::killEntity(Entity entity) {
		world_.destroyEntity(entity);
	}

	void EntitiesManager::killGroupEntity(Entity entity) {
		if (hasGroupIdByEntity(entity)) {
			std::vector<Entity> entities = getEntitiesByGroupId(getGroupIdByEntity(entity));
			for (auto entity : entities) {
				killEntity(entity);
			}
		}
	}


	bool EntitiesManager::hasEntityById(Entity::ID id) {
		return std::find(validId.begin(), validId.end(), id) != validId.end();
	}

	Entity EntitiesManager::getEntityById(Entity::ID id) {
		assert(hasEntityById(id));
		Entity entity(id);
		entity.entitiesManager_ = this;
		return entity;
	}

	/** TAG FUNCTION **/

	bool EntitiesManager::hasTagIdByEntity(Entity entity) const {
		return taggedEntityId.find(entity.id_) != taggedEntityId.end();
	}

	TagId EntitiesManager::getTagIdByEntity(Entity entity) {
		assert(hasTagIdByEntity(entity));
		return taggedEntityId[entity.id_];
	}

	bool EntitiesManager::hasEntityByTagId(TagId tagId) const {
		return taggedEntities.find(tagId) != taggedEntities.end();
	}

	Entity EntitiesManager::getEntityByTagId(TagId tagId) {
		assert(hasEntityByTagId(tagId));
		return taggedEntities[tagId];
	}

	void EntitiesManager::tagEntityByTagId(Entity entity,
										   TagId tagId) {
		taggedEntities[tagId] = entity;
		taggedEntityId[entity.id_] = tagId;
	}

	/** GROUP FUNCTION */

	bool
	EntitiesManager::hasEntitiesGroupId(TagId tagId) const {
		return groupedEntities.find(tagId) != groupedEntities.end() &&
				!groupedEntities.at(tagId).empty();
	}

	std::vector<Entity>
	EntitiesManager::getEntitiesByGroupId(TagId tagId) {
		assert(hasEntitiesGroupId(tagId));
		return groupedEntities[tagId];
	}

	void EntitiesManager::groupEntityByGroupId(Entity entity,
											   TagId tagId) {
		groupedEntityId[entity.id_] = tagId;
		groupedEntities[tagId].push_back(entity);
	}

	bool EntitiesManager::hasGroupIdByEntity(Entity entity) const {
		return groupedEntityId.find(entity.id_) != groupedEntityId.end();
	}

	TagId EntitiesManager::getGroupIdByEntity(Entity entity) {
		assert(hasGroupIdByEntity(entity));
		return groupedEntityId[entity.id_];
	}

}