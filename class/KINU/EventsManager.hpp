#pragma once

#include "Pool.hpp"
#include <unordered_map>
#include <typeindex>

namespace KINU {

	class World;

	class EventsManager {
	public:
		explicit EventsManager(World &world);

		template<typename T>
		void emitEvent(T event);

		template<typename T, typename ... Args>
		void emitEvent(Args &&... args);

		template<typename T>
		std::vector<T> getEvents();

		void destroyEvents();

	private:
		template<typename T>
		std::shared_ptr<Pool<T>> accommodateEvent();

		std::unordered_map<std::type_index, std::shared_ptr<AbstractPool>> eventPools;

		World &world_;
	};


	template<typename T>
	void EventsManager::emitEvent(T event) {
		std::shared_ptr<Pool<T>> eventPool = accommodateEvent<T>();
		eventPool->add(event);
	}

	template<typename T, typename ... Args>
	void EventsManager::emitEvent(Args &&... args) {
		T event(std::forward<Args>(args) ...);
		emitEvent<T>(event);
	}

	template<typename T>
	std::shared_ptr<Pool<T>> EventsManager::accommodateEvent() {
		if (eventPools.find(std::type_index(typeid(T))) == eventPools.end()) {
			std::shared_ptr<Pool<T>> pool(new Pool<T>());
			eventPools.insert(std::make_pair(std::type_index(typeid(T)), pool));
		}

		return std::static_pointer_cast<Pool<T>>(eventPools[std::type_index(typeid(T))]);
	}

	template<typename T>
	std::vector<T> EventsManager::getEvents() {
		if (eventPools.find(std::type_index(typeid(T))) == eventPools.end()) {
			std::shared_ptr<Pool<T>> pool(new Pool<T>());
			eventPools.insert(std::make_pair(std::type_index(typeid(T)), pool));
		}

		return std::static_pointer_cast<Pool<T>>(eventPools[std::type_index(typeid(T))])->getData();
	}

}