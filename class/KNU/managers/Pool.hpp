#pragma once

#include <vector>
#include <map>
#include <KNU/entities/Entity.hpp>
#include "IPool.hpp"

#define BASE_COMPONENT_SIZE 64

namespace KNU {

	template<typename ComponentType>
	class Pool : public  AbstractPool {
	private:
		std::vector<ComponentType> _pool;
		unsigned int capacity;
		unsigned int size;
	public:
		virtual void clear();

	private:

		// TODO   v   ENTITY SWITCH !
		std::map<Entity, unsigned int> _entitiesMap;
	public:

		virtual void add(Entity &entity, ComponentType &component) {
			unsigned int newInstance = size;
			if (size == capacity) {
				capacity += BASE_COMPONENT_SIZE;
				_pool.resize(capacity);
			}
			_pool[newInstance] = component;
			_entitiesMap[entity] = newInstance;
		}

		Pool();
		virtual ~Pool();
	};

	template<typename ComponentType>
	Pool<ComponentType>::Pool()
			:_pool(std::vector<ComponentType>(BASE_COMPONENT_SIZE)),
			 capacity(BASE_COMPONENT_SIZE),
			 size(0) {
	}

	template<typename ComponentType>
	Pool<ComponentType>::~Pool() {

	}

	template<typename ComponentType>
	void Pool<ComponentType>::clear() {

	}

}