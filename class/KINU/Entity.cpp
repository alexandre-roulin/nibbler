#include "Entity.hpp"
#include "World.hpp"

namespace KINU {

	/************** Entity **************/

	Entity::Entity() : id_(-1), entitiesManager(nullptr) {
		log_success("New entity no valid [%d]", id_);
	}

	Entity::Entity(Entity::ID id) : id_(id), entitiesManager(nullptr) {
		log_success("New entity valid [%d]", id_);
	}

	Entity::operator bool() {
		return id_ != -1;
	}

	Entity::ID Entity::getId() const {
		return id_;
	}

	bool Entity::isValid() const {
		return id_ != -1;
	}

	void Entity::invalidate() {
		id_ = -1;
	}

	bool Entity::operator<(const Entity &rhs) const {
		return id_ < rhs.id_;
	}

	bool Entity::operator>(const Entity &rhs) const {
		return rhs < *this;
	}

	bool Entity::operator<=(const Entity &rhs) const {
		return !(rhs < *this);
	}

	bool Entity::operator>=(const Entity &rhs) const {
		return !(*this < rhs);
	}

	bool Entity::operator==(const Entity &rhs) const {
		return id_ == rhs.id_;
	}

	bool Entity::operator!=(const Entity &rhs) const {
		return !(rhs == *this);
	}


	Entity &Entity::operator=(const Entity &rhs) {
		if (this != &rhs) {
			id_ = rhs.id_;
			entitiesManager = rhs.entitiesManager;
		}
		return *this;
	}

	void Entity::kill() {
		entitiesManager->killEntity(*this);
	}


	void Entity::killGroup() {
		entitiesManager->killGroup(this->getGroup());
	}

	void Entity::tag(std::string tag) {
		entitiesManager->tagEntity(*this, tag);
	}

	void Entity::group(std::string group) {
		entitiesManager->groupEntity(*this, group);
	}

	std::string &Entity::getTag() {
		return entitiesManager->getTagOfEntity(*this);
	}

	std::string &Entity::getGroup() {
		return entitiesManager->getGroupOfEntity(*this);
	}

	Signature &Entity::getSignature() {
		log_fatal("Entity::getSignature valid [%d] * [%d] id [%d]", isValid(), entitiesManager != nullptr, id_);
		return signature;
	}

	bool Entity::hasTag() {
		return entitiesManager->hasTag(*this);
	}

	bool Entity::hasGroup() {
		return entitiesManager->hasGroup(*this);
	}

	Entity::Entity(const Entity &rhs) {
		id_ = rhs.id_;
		entitiesManager = rhs.entitiesManager;
	}

	/************** EntitiesManager **************/

	EntitiesManager::EntitiesManager(World &world) :
			world_(world),
			size(0),
			capacity(BASE_ENTITY_CAPACITY) {
		entities.resize(capacity);
	}


	Entity EntitiesManager::createEntity() {
		Entity::ID entityId;
		if (!freeIds.empty()) {
			entityId = freeIds.front();
			freeIds.pop();
		} else {
			if (size >= capacity) {
				capacity += BASE_ENTITY_CAPACITY;
				entities.resize(capacity);
			}
			entityId = size++;
		}
		assert(tagId_[entityId].empty());
		Entity entity(entityId);
		entity.signature.clean();
		entity.entitiesManager = this;
		entities[entityId] = entity;
		return entity;
	}

	void EntitiesManager::deleteEntity(Entity &entity) {
		Entity::ID entityId = entity.getId();

		auto taggedEntity = tagId_.find(entityId);
		if (taggedEntity != tagId_.end()) {
			auto tag = taggedEntity->second;
			taggedEntities_.erase(tag);
			tagId_.erase(taggedEntity);
		}
		auto groupedEntity = groupId_.find(entityId);
		if (groupedEntity != groupId_.end()) {

			auto groupName = groupedEntity->second;

			auto group = groupedEntities_.find(groupName);
			if (group != groupedEntities_.end()) {
				auto entityInGroup = group->second.find(entity);
				if (entityInGroup != group->second.end()) {
					group->second.erase(entityInGroup);
				}
			}
			groupId_.erase(groupedEntity);
		}
		freeIds.push(entityId);
		entity.invalidate();
	}

	void EntitiesManager::killEntity(Entity &entity) {
		world_.destroyEntity(entity);
	}

	void EntitiesManager::killGroup(std::string group) {
		auto &e = groupedEntities_[group];
		std::for_each(groupedEntities_[group].begin(),
					  groupedEntities_[group].end(),
					  [](Entity entity) {
							entity.kill();
					  });
	}

	void EntitiesManager::tagEntity(Entity &entity, std::string tag) {
		tagId_.emplace(entity.id_, tag);
		taggedEntities_.emplace(tag, entity);
	}

	void EntitiesManager::groupEntity(Entity &entity, std::string group) {
		groupedEntities_.emplace(group, std::set<Entity>());
		groupedEntities_[group].emplace(entity);
		groupId_.emplace(entity.id_, group);
	}

	Entity &EntitiesManager::getEntityByTag(std::string tag) {
		return taggedEntities_[tag];
	}

	Entity &EntitiesManager::getEntityById(Entity::ID id) {
		log_success("getEntityById %d %d ", entities.size(), id);
		return entities[id];
	}

	std::vector<Entity> EntitiesManager::getEntitiesByGroup(std::string group) {
		auto &groupSet = groupedEntities_[group];
		return std::vector<Entity>(groupSet.begin(), groupSet.end());
	}

	std::string &EntitiesManager::getTagOfEntity(Entity &entity) {
		return tagId_[entity.id_];
	}

	std::string &EntitiesManager::getGroupOfEntity(Entity &entity) {
		return groupId_[entity.id_];
	}

	bool EntitiesManager::hasTag(Entity &entity) {
		return tagId_[entity.id_].compare("") != 0;
	}

	bool EntitiesManager::hasGroup(Entity &entity) {
		return groupId_[entity.id_].compare("") != 0;
	}
}