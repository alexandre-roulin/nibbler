#pragma once

#include <KNU/entities/Entity.hpp>
#include "ComponentManager.hpp"
#include <vector>
#include <unordered_map>
#include <set>
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

	class EntitiesManager {
	private:
		unsigned int size;
		unsigned int capacity;
		World &world;
		ComponentManager componentManager;
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
		T &getComponent(Entity &e) {
			assert(e.mask.getMask() & Component<T>::signature());
			return componentManager.getComponent<T>(e);
		}

		template<typename T>
		bool hasComponent(Entity &e) {
			return componentManager.hasComponent<T>(e);
		}

		template<typename T>
		EntitiesManager &addComponent(Entity &e, T component) {
			e.mask.addComponent<T>();
			componentManager.addComponent(e, component);
			return *this;
		}

		template<typename T, typename ... Args>
		EntitiesManager &addComponent(Entity &e, Args &&... args) {
			T component(std::forward<Args>(args) ...);
			addComponent<T>(e, component);
			return *this;
		}

		template<typename T>
		void removeComponent(Entity &e) {
			e.mask.removeComponent<T>();
			componentManager.removeComponent<T>(e);
		}

	};

}