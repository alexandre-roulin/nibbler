#pragma once

#include <vector>
#include <unordered_map>
#include <set>
#include <KNU/entities/Entity.hpp>
// [*][*][*]                    [S][S][S][S {0} ]
//  |  \
//  |   \
//  |    \
//  |     \
// data    \
//          \
//         data

namespace KNU {

	class World;
	class ComponentManager;
	class EntitiesManager {
	private:
		unsigned int size;
		unsigned int capacity;
		World &world;
		ComponentManager const &_componentManager;
		std::vector<Entity> _entitiesMap;
		std::unordered_map<std::string, Entity> _taggedEntity;
		std::unordered_map<std::string, std::set<Entity>> _groupedEntities;
	public:
		EntitiesManager() = delete;

		explicit EntitiesManager(World &world);

		Entity &createEntity();

		void destroyEntity(Entity &entity);

		void tagEntity(Entity &e, std::string &tag);

		bool isEntityAlive(Entity e) const;

		Entity getEntityByTag(std::string const &tag);

		bool hasTaggedEntity(std::string &tag);

		bool hasTag(std::string const &tag);

		bool hasGroup(std::string const &group);

		void groupEntity(Entity &e, std::string &group);

		std::vector<Entity> getEntitiesByGroup(std::string const &group);

		unsigned long getGroupSize(std::string group);

		void killEntity(Entity &e);

		template<typename T>
		T &getComponent(Entity &e);

		template<typename T>
		bool hasComponent(Entity const &e);

		template<typename T>
		EntitiesManager &addComponent(Entity &e, T component);

		template<typename T, typename ... Args>
		EntitiesManager &addComponent(Entity &e, Args &&... args);

		template<typename T>
		void removeComponent(Entity &e);

	};

}

