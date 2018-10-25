#ifndef __COMPONENT_MANAGER_HPP__
#define __COMPONENT_MANAGER_HPP__
#include <map>
#include <iostream>
#include <vector>
#include "Pool.hpp"
#include "AbstractPool.hpp"

namespace KNU {
	class Entity;


	class ComponentManager {
	private:
		std::vector<std::shared_ptr<AbstractPool>> componentPools;

	public:
		ComponentManager();
		ComponentManager(ComponentManager const &);
		template<typename T>
		void addComponent(Entity &e, T component);

		template<typename T>
		T &getComponent(Entity &entity);

		template<typename T>
		void removeComponent(Entity &entity);

		template<typename T>
		bool hasComponent(Entity const &entity);

		template<typename T>
		std::shared_ptr<Pool<T>> accommodateComponent();
	};


}

#endif //__COMPONENT_MANAGER_HPP__