#include "Entity.hpp"
#include <KNU/utils/Signature.hpp>

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

	EntitiesManager::EntitiesManager(World &world, ComponentManager &componentManager)
			: _componentManager(componentManager),
			  world(world),
			  size(0),
			  capacity(BASE_ENTITIES_CAPACITY) {
		_entitiesMap.resize(capacity);
	}


/** Entities Managements **/

	Entity &
	EntitiesManager::createEntity() {
		if (size == capacity) {
			capacity += BASE_ENTITIES_CAPACITY;
			_entitiesMap.resize(capacity);
		}
		unsigned int instance_entity = size;
		size++;
		_entitiesMap[instance_entity] = Entity(instance_entity);
		_entitiesMap[instance_entity].entityManager = this;
		return _entitiesMap[instance_entity];
	}

	void EntitiesManager::destroyEntity(Entity &entity) {
		assert(entity.id < size);
		assert(entity == _entitiesMap[entity.id]);
		size--;
		_entitiesMap[entity.id] = _entitiesMap[size];
	}

	void EntitiesManager::killEntity(Entity &e) {
		e.alive = false;
	}

/** Tag/Group Management **/
	bool EntitiesManager::hasTaggedEntity(std::string &tag) {
		return _taggedEntity.find(tag) != _taggedEntity.end();
	}

	void EntitiesManager::tagEntity(Entity &e, std::string &tag) {
		_taggedEntity.insert(std::make_pair(tag, e));
	}

	void EntitiesManager::groupEntity(Entity &e, std::string &group) {
		if (!hasGroup(group)) {
			_groupedEntities.emplace(std::make_pair(group, std::set<Entity>()));
		}
		_groupedEntities[group].emplace(e);
	}


	std::vector<Entity>
	EntitiesManager::getEntitiesByGroup(std::string const &group) {
		if (!hasGroup(group))
			return std::vector<Entity>();
		auto &set = _groupedEntities[group];
		return std::vector<Entity>(set.begin(), set.end());
	}

	unsigned long EntitiesManager::getGroupSize(std::string group) {
		return _groupedEntities.size();
	}

	bool EntitiesManager::hasTag(std::string const &tag) {
		return _taggedEntity.find(tag) != _taggedEntity.end();
	}

	bool EntitiesManager::hasGroup(std::string const &group) {
		return _groupedEntities.find(group) != _groupedEntities.end();
	}

	bool EntitiesManager::isEntityAlive(const Entity e) const {
		return e.alive;
	}

	Entity EntitiesManager::getEntityByTag(std::string const &tag) {
		assert(hasTag(tag));
		return _taggedEntity[tag];
	}



}