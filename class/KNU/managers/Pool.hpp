#pragma once

#include <vector>
#include <map>
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

		void add(Entity const &entity, T &component);

		T &get(Entity const &e);

		bool has(Entity const &e);

		void remove(Entity const &e);

		virtual ~Pool();
	};
}