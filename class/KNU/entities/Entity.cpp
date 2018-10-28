#include <utility>

#include "Entity.hpp"
#include <KNU/utils/Signature.hpp>
#include <KNU/World.hpp>
#include <logger.h>
#include <component/CollisionComponent.hpp>
#include <component/FollowComponent.hpp>
#include <component/JoystickComponent.hpp>
#include <component/MotionComponent.hpp>
#include <component/PositionComponent.hpp>
#include <component/SpriteComponent.hpp>

namespace KNU {


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
		getEntitiesManager().groupEntity(*this, group);
	}

	bool Entity::hasGroup(std::string group) const {
		return getEntitiesManager().hasEntityInGroup(*this, std::move(group));
	}

	Signature &Entity::getSignature() {
		return entityManager->getEntitySignature(*this);
	}

	Entity::ID Entity::getId() const {
		return getEntitiesManager().getEntityId(*this);
	}

	Entity &Entity::operator=(Entity const &entity) {
		if (this != &entity) {
			id = entity.id;
			this->alive = entity.alive;
			entityManager = entity.entityManager;
		}
		return *this;
	}

	EntitiesManager::EntitiesManager(World &world)
			: size(0),
			  capacity(BASE_ENTITIES_CAPACITY),
			  world(world),
			  _componentManager(ComponentManager()) {
		_entitiesMap.resize(capacity);
		_poolSignature.resize(capacity);
	}

	ComponentManager::ComponentManager() {
		log_success("ComponentManager");
		componentPools.resize(COMPONENT_MAX, nullptr);
		componentPools.size();
	}

/** Entities Managements **/
	Json::Value EntitiesManager::serializeComponent() const {
		Json::Value root;
		std::for_each(_entitiesMap.begin(), _entitiesMap.end(),
					  [&root](Entity const &entity) {
						  if (entity.getId() != -1) {
							  log_error("CollisionComponent");
							  if (entity.hasComponent<CollisionComponent>()) {
								  auto collisionComponent = entity.getComponent<CollisionComponent>();
								  root[std::to_string(entity.getId())].append(
										  collisionComponent.serializeComponent());
							  }
							  log_error("FollowComponent");
							  if (entity.hasComponent<FollowComponent>()) {
								  auto followComponent = entity.getComponent<FollowComponent>();
								  root[std::to_string(entity.getId())].append(
										  followComponent.serializeComponent());
							  }
							  log_error("JoystickComponent");
							  if (entity.hasComponent<JoystickComponent>()) {
								  auto joystickComponent = entity.getComponent<JoystickComponent>();
								  root[std::to_string(entity.getId())].append(
										  joystickComponent.serializeComponent());
							  }
							  log_error("MotionComponent");
							  if (entity.hasComponent<MotionComponent>()) {
								  auto motionComponent = entity.getComponent<MotionComponent>();
								  root[std::to_string(entity.getId())].append(
										  motionComponent.serializeComponent());
							  }
							  log_error("PositionComponent");
							  if (entity.hasComponent<PositionComponent>()) {
								  log_error("has");
								  auto positionComponent = entity.getComponent<PositionComponent>();
								  log_error("get");
								  root[std::to_string(entity.getId())].append(
										  positionComponent.serializeComponent());
								  log_error("append");
							  }
							  log_error("SpriteComponent");

//							  if (entity.hasComponent<SpriteComponent>()) {
//								  auto collisionComponent = entity.getComponent<SpriteComponent>();
//								  root[std::to_string(entity.getId())] = collisionComponent.serializeComponent();
//							  }
						  }
					  });
		for (auto json = root.begin(); json != root.end(); ++json) {
			std::cout << json.key() << std::endl;
			std::cout << json.name() << std::endl;
		}
		return root;
	}

	Entity &
	EntitiesManager::createEntity() {
		Entity::ID instance_entity;
		if (!freeIds.empty()) {
			instance_entity = freeIds.front();
		} else {
			if (size == capacity) {
				capacity += BASE_ENTITIES_CAPACITY;
				_entitiesMap.resize(capacity);
				_poolSignature.resize(capacity);
			}
			instance_entity = static_cast<short>(size);
			size++;
		}

		_entitiesMap[instance_entity] = Entity(instance_entity);
		_poolSignature[instance_entity] = Signature();
		_entitiesMap[instance_entity].entityManager = this;
		return _entitiesMap[instance_entity];
	}

	void EntitiesManager::destroyEntity(Entity &entity) {
		log_success("EntitiesManager::destroyEntity");
		freeIds.push(entity.getId());
		_poolSignature[entity.id].clean();
		for (auto it = _taggedEntity.begin(); it != _taggedEntity.end();)
			if (it->second == entity) it = _taggedEntity.erase(it); else ++it;

		for (auto &groupEntity : _groupedEntities) {
			auto find = groupEntity.second.find(entity);
			if (find != groupEntity.second.end())
				groupEntity.second.erase(find);
		}
	}

	void EntitiesManager::killEntity(Entity &entity) {
		world.destroyEntity(entity);
		entity.alive = false;
	}

/** Tag/Group Management **/

	Entity EntitiesManager::getEntityById(Entity::ID id) const {
		return _entitiesMap[id];
	}


	bool EntitiesManager::hasTaggedEntity(std::string &tag) {
		return _taggedEntity.find(tag) != _taggedEntity.end();
	}

	void EntitiesManager::tagEntity(Entity &entity, std::string &tag) {
		_taggedEntity.insert(std::make_pair(tag, entity));
	}

	void EntitiesManager::groupEntity(Entity &entity, std::string &group) {
		if (!hasGroup(group)) {
			_groupedEntities.emplace(std::make_pair(group, std::set<Entity>()));
		}
		_groupedEntities[group].emplace(entity);
	}


	std::vector<Entity>
	EntitiesManager::getEntitiesByGroup(std::string const &group) {
		if (!hasGroup(group))
			return std::vector<Entity>();
		auto &set = _groupedEntities[group];
		return std::vector<Entity>(set.begin(), set.end());
	}

	unsigned long EntitiesManager::getGroupSize(std::string group) {
		return _groupedEntities[group].size();
	}

	bool EntitiesManager::hasTag(std::string const &tag) {
		return _taggedEntity.find(tag) != _taggedEntity.end();
	}

	bool EntitiesManager::hasGroup(std::string const &group) {
		return _groupedEntities.find(group) != _groupedEntities.end();
	}

	bool
	EntitiesManager::hasEntityInGroup(Entity const &entity, std::string group) {
		auto it = _groupedEntities.find(group);
		if (it != _groupedEntities.end()) {
			if (it->second.find(entity.id) != it->second.end()) {
				return true;
			}
		}
		return false;
	}

	bool EntitiesManager::isEntityAlive(Entity const &entity) const {
		return entity.alive;
	}

	Entity EntitiesManager::getEntityByTag(std::string const &tag) {
		assert(hasTag(tag));
		return _taggedEntity[tag];
	}

	Signature &EntitiesManager::getEntitySignature(Entity &entity) {
		return _poolSignature[entity.id];
	}

	Entity::ID EntitiesManager::getEntityId(Entity const &e) const {
		return e.id;
	}
}