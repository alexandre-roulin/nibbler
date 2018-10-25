#pragma once

#include <vector>
#include <map>
#include <KNU/entities/Entity.hpp>
#include <iostream>
#include "AbstractPool.hpp"

#define BASE_COMPONENT_SIZE 64

namespace KNU {

	class Entity;

	template<typename T>
	class Pool : public AbstractPool {
	private:
		std::vector<T> _pool;
		unsigned int capacity;
		unsigned int size;
		std::map<Entity, unsigned int> _entitiesMap;
	public:

		Pool();

		void add(Entity &entity, T &component) {
			unsigned int newInstance = size;
			if (size == capacity) {
				capacity += BASE_COMPONENT_SIZE;
				_pool.resize(capacity);
			}
			_pool[newInstance] = component;
			_entitiesMap[entity] = newInstance;
			size++;
		}

		T &get(Entity &e) {
			unsigned int instance = _entitiesMap.at(e);
			std::cout << instance << std::endl;
			assert(instance < capacity);
			return _pool[instance];
		}

		bool has(Entity &e) {
			return _entitiesMap.find(e) != _entitiesMap.end();
		}

		void remove(Entity &e) {
			unsigned int instance = _entitiesMap.at(e);
			assert(instance < size);
			_entitiesMap.erase(e);
			size--;
			std::swap(_pool[instance], _pool[size]);
		}

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

}