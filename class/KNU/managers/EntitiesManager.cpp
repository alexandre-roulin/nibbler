#include <nibbler.hpp>
#include <iostream>
#include "EntitiesManager.hpp"

/** Entities Managements **/

KNU::EntitiesManager::EntitiesManager()
		: size(0),
		  capacity(BASE_ENTITIES_CAPACITY) {
	_entitiesMap.resize(capacity);
}

KNU::Entity &
KNU::EntitiesManager::createEntity() {
	if (size == capacity) {
		capacity += BASE_ENTITIES_CAPACITY;
		_entitiesMap.resize(capacity);
	}
	unsigned int instance_entity = size;
	size++;
	_entitiesMap[instance_entity] = Entity(instance_entity);
	return _entitiesMap[instance_entity];
}

void KNU::EntitiesManager::destroyEntity(KNU::Entity &entity) {
	assert(entity.id < size);
	assert(entity == _entitiesMap[entity.id]);
	size--;
	_entitiesMap[entity.id] = _entitiesMap[size];
}

bool KNU::EntitiesManager::isEntityAlive(KNU::Entity &e) {
	return false;
}

void KNU::EntitiesManager::killEntity(KNU::Entity &e) {
	e.alive = false;
}

/** Tag/Group Management **/

void KNU::EntitiesManager::tagEntity(Entity &e, std::string &tag) {
	_taggedEntity.insert(std::make_pair(tag, e));
}

void KNU::EntitiesManager::groupEntity(KNU::Entity &e, std::string &group) {
	if (!hasGroup(group)) {
		_groupedEntities.emplace(std::make_pair(group, std::set<Entity>()));
	}
	_groupedEntities[group].emplace(e);
}

KNU::Entity &KNU::EntitiesManager::getEntityByTag(std::string const &tag) {
	assert(hasTag(tag));
	return _taggedEntity.at(tag);
}


std::vector<KNU::Entity>
KNU::EntitiesManager::getEntitiesByGroup(std::string const &group) {
	assert(hasGroup(group));
	auto &set = _groupedEntities[group];
	return std::vector<Entity>(set.begin(), set.end());
}

unsigned long KNU::EntitiesManager::getGroupSize(std::string group) {
	return _groupedEntities.size();
}

bool KNU::EntitiesManager::hasTag(std::string const &tag) {
	return _taggedEntity.find(tag) != _taggedEntity.end();
}

bool KNU::EntitiesManager::hasGroup(std::string const &group) {
	return _groupedEntities.find(group) != _groupedEntities.end();
}

/** Component Mangement **/








