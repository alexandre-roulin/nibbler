#pragma once

#include <vector>
#include <map>
#include <iostream>
#include "AbstractPool.hpp"

#define BASE_COMPONENT_SIZE 10

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

		virtual void clean();

		void add(Entity const &entity, T &component);

		T &get(Entity const &e);

		bool has(Entity const &e);

		void remove(Entity const &e);

		virtual ~Pool();
	};

	template<typename T>
	class Pool;

	template<typename T>
	Pool<T>::Pool()
			:_pool(std::vector<T>(BASE_COMPONENT_SIZE)),
			 capacity(BASE_COMPONENT_SIZE),
			 size(0) {
	}

	template<typename T>
	void Pool<T>::add(Entity const &entity, T &component) {
		unsigned int newInstance = size;
		if (size == capacity) {
			capacity += BASE_COMPONENT_SIZE;
			_pool.resize(capacity);
		}
		_pool[newInstance] = component;
		_entitiesMap[entity] = newInstance;
		size++;
	}

	template<typename T>
	T &Pool<T>::get(Entity const &e) {
		unsigned int instance = _entitiesMap.at(e);
		assert(instance < capacity);
		return _pool[instance];
	}

	template<typename T>
	bool Pool<T>::has(Entity const &e) {
		return _entitiesMap.find(e) != _entitiesMap.end();
	}

	template<typename T>
	void Pool<T>::remove(Entity const &e) {

		unsigned int instance = _entitiesMap.at(e);
		assert(instance < size);
		_entitiesMap.erase(e);
		size--;
		std::swap(_pool[instance], _pool[size]);
	}

	template<typename T>
	Pool<T>::~Pool() {

	}

	template<typename T>
	void Pool<T>::clean() {
		_pool.clear();
	}
}