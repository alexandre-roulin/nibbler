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

	bool Entity
	::isAlive() const {
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

	std::string Entity::getTag() const {
		return getEntitiesManager().getTagOfEntity(*this);
	}

	std::string Entity::getGroup() const {
		return getEntitiesManager().getGroupOfEntity(*this);
	}

	std::string const EntitiesManager::TAG = "tag";
	std::string const EntitiesManager::GROUP = "group";

	EntitiesManager::EntitiesManager(World &world)
			: size(0),
			  capacity(BASE_ENTITIES_CAPACITY),
			  world(world),
			  _componentManager(ComponentManager()) {
		_entitiesMap.resize(capacity);
		_poolSignature.resize(capacity);
	}

	ComponentManager::ComponentManager() {
		componentPools.resize(COMPONENT_MAX, nullptr);
		componentPools.size();
	}

/** Entities Managements **/

	void EntitiesManager::deserializeKNU(Json::Value json) {
		for (auto ite = json.begin(); ite != json.end(); ++ite) {
			auto entity = createEntity(static_cast<KNU::Entity::ID>
									   (std::stoi(ite.key().asString())));
			for (auto itc = json[ite.name()].begin();
				 itc != json[ite.name()].end(); ++itc) {
				auto value = json[ite.name()][itc.name()];
				if (itc.name() == CollisionComponent::NAME_COMPONENT)
					entity.addComponent(CollisionComponent(value));
				else if (itc.name() == FollowComponent::NAME_COMPONENT)
					entity.addComponent(FollowComponent(value));
				else if (itc.name() == JoystickComponent::NAME_COMPONENT)
					entity.addComponent(JoystickComponent(value));
				else if (itc.name() == MotionComponent::NAME_COMPONENT)
					entity.addComponent(MotionComponent(value));
				else if (itc.name() == PositionComponent::NAME_COMPONENT)
					entity.addComponent(PositionComponent(value));
				else if (itc.name() == EntitiesManager::TAG)
					tagEntity(entity, value.asString());
				else if (itc.name() == EntitiesManager::GROUP)
					groupEntity(entity, value.asString());
			}
			world.createdEntities_.push_back(entity);
		}
		for (int index = 0; index < capacity; ++index) {
			if (_entitiesMap[index].id == -1)
				freeIds.push(index);
		}
	}

	Json::Value EntitiesManager::serializeKNU() const {
		Json::Value root;
		std::for_each(_entitiesMap.begin(), _entitiesMap.end(),
					  [&root, this](Entity const &entity) {
						  if (entity.getId() != -1) {

							  if (entity.hasComponent<CollisionComponent>()) {
								  auto collisionComponent = entity.getComponent<CollisionComponent>();
								  root[std::to_string(entity.getId())]
								  [CollisionComponent::NAME_COMPONENT]
										  = collisionComponent.serializeComponent();
							  }
							  if (entity.hasComponent<FollowComponent>()) {
								  auto followComponent = entity.getComponent<FollowComponent>();
								  root[std::to_string(entity.getId())]
								  [FollowComponent::NAME_COMPONENT]
										  = followComponent.serializeComponent();
							  }
							  if (entity.hasComponent<JoystickComponent>()) {
								  auto joystickComponent = entity.getComponent<JoystickComponent>();
								  root[std::to_string(entity.getId())]
								  [JoystickComponent::NAME_COMPONENT]
										  = joystickComponent.serializeComponent();
							  }
							  if (entity.hasComponent<MotionComponent>()) {
								  auto motionComponent = entity.getComponent<MotionComponent>();
								  root[std::to_string(entity.getId())]
								  [MotionComponent::NAME_COMPONENT]
										  = motionComponent.serializeComponent();
							  }
							  if (entity.hasComponent<PositionComponent>()) {
								  auto positionComponent = entity.getComponent<PositionComponent>();
								  root[std::to_string(entity.getId())]
								  [PositionComponent::NAME_COMPONENT]
										  = positionComponent.serializeComponent();
							  }
							  root[std::to_string(
									  entity.getId())][EntitiesManager::TAG] =
									  getTagOfEntity(entity);
							  root[std::to_string(
									  entity.getId())][EntitiesManager::GROUP] =
									  getGroupOfEntity(entity);
						  }
					  });
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

	Entity &EntitiesManager::createEntity(Entity::ID instance_entity) {
		if (instance_entity > capacity) {
			capacity += BASE_ENTITIES_CAPACITY;
			_entitiesMap.resize(capacity);
			_poolSignature.resize(capacity);
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

	std::string EntitiesManager::getTagOfEntity(Entity const &entity) const {
		for (auto &pair : _taggedEntity) {
			if (pair.second == entity)
				return pair.first;
		}
		return "";
	}

	std::string EntitiesManager::getGroupOfEntity(Entity const &entity) const {
		for (auto &pair : _groupedEntities) {
			if (pair.second.find(entity) != pair.second.end())
				return pair.first;
		}
		return "";
	}

	bool EntitiesManager::hasTaggedEntity(std::string &tag) {
		return _taggedEntity.find(tag) != _taggedEntity.end();
	}

	void EntitiesManager::tagEntity(Entity &entity, std::string tag) {
		_taggedEntity.insert(std::make_pair(tag, entity));
	}

	void EntitiesManager::groupEntity(Entity &entity, std::string group) {
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
//		assert(hasTag(tag));
		return _taggedEntity[tag];
	}

	Signature &EntitiesManager::getEntitySignature(Entity &entity) {
		return _poolSignature[entity.id];
	}

	Entity::ID EntitiesManager::getEntityId(Entity const &e) const {
		return e.id;
	}
}