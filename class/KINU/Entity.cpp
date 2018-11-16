#include "Entity.hpp"
#include "World.hpp"
#include <logger.h>

namespace KINU {

	void Entity::kill() {
		getEntityManager().killEntity(*this);
	}


	void Entity::killGroup() {
		getEntityManager().killEntitiesGroup(getGroup());
	}

	bool Entity::isAlive() const {
		return getEntityManager().isEntityAlive(*this);
	}

	void Entity::tag(std::string tag) {
		getEntityManager().tagEntity(*this, tag);
	}


	bool Entity::hasTag(std::string tag) const {
		return getEntityManager().hasTaggedEntity(tag, *this);
	}

	std::string Entity::getTag() const {
		return getEntityManager().getTagOfEntity(*this);
	}

	std::string Entity::getGroup() const {
		return getEntityManager().getGroupOfEntity(*this);
	}

	void Entity::group(std::string group) {
		getEntityManager().groupEntity(*this, group);
	}

	bool Entity::hasGroup(std::string group) const {
		return getEntityManager().hasEntityInGroup(group, *this);
	}

	std::string Entity::toString() const {
		std::string s =
				"entity id: " + std::to_string(getIndex()) + ", version: " +
				std::to_string(getVersion());
		return s;
	}

	EntityManager::EntityManager(World &world) :world(world){

	}

	EntityManager &Entity::getEntityManager() const {
		assert(entityManager != nullptr);
		return *entityManager;
	}

	Entity EntityManager::createEntity() {
		Entity::Id index;

		if (freeIds.size() > MinimumFreeIds) {
			index = freeIds.front();
			freeIds.pop_front();
		} else {
			versions.push_back(0);
			index = (unsigned int) versions.size() - 1;
			assert(index < (1 << Entity::IndexBits));
			log_warn("New Index %d with %d", index, componentMasks.size());
			if (index >= componentMasks.size()) {
				componentMasks.resize(componentMasks.size() + DEFAULT_POOL_SIZE);
				log_warn("RESIZE componentMasks > %d", componentMasks.size());
			}
		}

		assert(index < versions.size());
		Entity e(index, versions[index]);
		e.entityManager = this;
		return e;
	}

	void EntityManager::destroyEntity(Entity e) {
		const auto index = e.getIndex();
		assert(index < versions.size());        // sanity check
		assert(index < componentMasks.size());
		++versions[index];                      // increase the version for that id
		freeIds.push_back(
				index);               // make the id available for reuse
		componentMasks[index].reset();          // reset the component mask for that id

		// if tagged, remove entity from tag management
		auto taggedEntity = entityTags.find(e.id);
		if (taggedEntity != entityTags.end()) {
			auto tag = taggedEntity->second;
			taggedEntities.erase(tag);
			entityTags.erase(taggedEntity);
		}

		// if in group, remove entity from group management
		auto groupedEntity = entityGroups.find(e.id);
		if (groupedEntity != entityGroups.end()) {
			auto groupName = groupedEntity->second;
			auto group = groupedEntities.find(groupName);
			if (group != groupedEntities.end()) {
				auto entityInGroup = group->second.find(e);
				if (entityInGroup != group->second.end()) {
					group->second.erase(entityInGroup);
				}
			}
			entityGroups.erase(groupedEntity);
		}
	}

	void EntityManager::killEntity(Entity e) {
		world.destroyEntity(e);
	}


	void EntityManager::killEntitiesGroup(std::string group) {
		auto &entitiesInGroup = groupedEntities[group];

		std::for_each(entitiesInGroup.begin(), entitiesInGroup.end(),
					  [](Entity entity) {
							entity.kill();
					  });
	}

	bool EntityManager::isEntityAlive(Entity e) const {
		const auto index = e.getIndex();
		assert(index < versions.size());
		return versions[index] == e.getVersion();
	}

	Entity EntityManager::getEntity(Entity::Id index) {
		assert(index < versions.size());
		Entity e(index, versions[index]);
		e.entityManager = this;
		return e;
	}

	const ComponentMask &EntityManager::getComponentMask(Entity e) const {
		const auto index = e.getIndex();
		assert(index < componentMasks.size());
		return componentMasks[index];
	}


	std::string EntityManager::getGroupOfEntity(Entity entity) const{
		auto groupedEntity = entityGroups.find(entity.id);
		if (groupedEntity != entityGroups.end()) {
			return groupedEntity->second;
		}
		return "";
	}

	std::string EntityManager::getTagOfEntity(Entity entity) const{
		auto taggedEntity = entityTags.find(entity.id);
		if (taggedEntity != entityTags.end()) {
			return taggedEntity->second;
		}
		return "";
	}

	void EntityManager::tagEntity(Entity e, std::string tag) {
		taggedEntities.emplace(tag, e);
		entityTags.emplace(e.id, tag);
	}

	bool EntityManager::hasTag(std::string tag) const {
		return taggedEntities.find(tag) != taggedEntities.end();
	}

	bool EntityManager::hasTaggedEntity(std::string tag, Entity e) const {
		auto it = taggedEntities.find(tag);
		if (it != taggedEntities.end()) {
			if (it->second == e) {
				return true;
			}
		}
		return false;
	}

	Entity EntityManager::getEntityByTag(std::string tag) {
		assert(hasTag(tag));
		return taggedEntities[tag];
	}

	int EntityManager::getTagCount() const {
		return taggedEntities.size();
	}

	void EntityManager::groupEntity(Entity e, std::string group) {
		groupedEntities.emplace(group, std::set<Entity>());
		groupedEntities[group].emplace(e);
		entityGroups.emplace(e.id, group);
	}

	bool EntityManager::hasGroup(std::string group) const {
		return groupedEntities.find(group) != groupedEntities.end();
	}

	bool EntityManager::hasEntityInGroup(std::string group, Entity e) const {
		auto it = groupedEntities.find(group);
		if (it != groupedEntities.end()) {
			if (it->second.find(e.id) != it->second.end()) {
				return true;
			}
		}
		return false;
	}

	std::vector<Entity> EntityManager::getEntityGroup(std::string group) {
		assert(hasGroup(group));
		auto &s = groupedEntities[group];
		return std::vector<Entity>(s.begin(), s.end());
	}

	int EntityManager::getGroupCount() const {
		return groupedEntities.size();
	}

	int EntityManager::getEntityGroupCount(std::string group) {
		if (!hasGroup(group)) {
			return 0;
		}

		return groupedEntities[group].size();
	}

}
