#pragma once

#include <vector>
#include <map>
#include <KNU/entities/Entity.hpp>
#include "IPool.hpp"

#define BASE_COMPONENT_SIZE 64

namespace KNU {

	template<typename T>
	class Pool : public  AbstractPool {
	private:
		std::vector<T> _pool;
		unsigned int capacity;
		unsigned int size;
	public:
		virtual void clear();

		std::map<Entity, unsigned int> _entitiesMap;

		void add(Entity &entity, T &component) {
			unsigned int newInstance = size;
			if (size == capacity) {
				capacity += BASE_COMPONENT_SIZE;
				_pool.resize(capacity);
			}
			_pool[newInstance] = component;
			_entitiesMap[entity] = newInstance;
		}

		T &get(Entity &e) {
			assert(e.id < capacity);
			unsigned int instance = _entitiesMap[e];
			assert(instance < capacity);
			return _pool[instance];
		}

		Pool();
		virtual ~Pool();
	};

	template<typename T>
	Pool<T>::Pool()
			:_pool(std::vector<T>(BASE_COMPONENT_SIZE)),
			 capacity(BASE_COMPONENT_SIZE),
			 size(0) {
	}

	template<typename T>
	Pool<T>::~Pool() {

	}

	template<typename T>
	void Pool<T>::clear() {

	}

}