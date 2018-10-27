#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include <cstdint>
#include <KNU/managers/AbstractPool.hpp>
#include <KNU/managers/Pool_.hpp>

namespace KNU {

	class World;


	class EventManager {
	public:
		explicit EventManager(World &world);

		template<typename T>
		void emitEvent(T event);

		template<typename T, typename ... Args>
		void emitEvent(Args &&... args);

		template<typename T>
		std::vector<T> getEvents();

		void destroyEvents();

	private:
		template<typename T>
		std::shared_ptr<Pool_<T>> accommodateEvent();

		std::unordered_map<std::type_index, std::shared_ptr<AbstractPool>> eventPools;

		World &world;
	};

	template<typename T>
	void EventManager::emitEvent(T event) {
		std::shared_ptr<Pool_<T>> eventPool = accommodateEvent<T>();
		eventPool->add(event);
	}

	template<typename T, typename ... Args>
	void EventManager::emitEvent(Args &&... args) {
		T event(std::forward<Args>(args) ...);
		emitEvent<T>(event);
	}

	template<typename T>
	std::shared_ptr<Pool_<T>> EventManager::accommodateEvent() {
		if (eventPools.find(std::type_index(typeid(T))) == eventPools.end()) {
			std::shared_ptr<Pool_<T>> pool(new Pool_<T>());
			eventPools.insert(std::make_pair(std::type_index(typeid(T)), pool));
		}

		return std::static_pointer_cast<Pool_<T>>(
				eventPools[std::type_index(typeid(T))]);
	}

	template<typename T>
	std::vector<T> EventManager::getEvents() {
		if (eventPools.find(std::type_index(typeid(T))) == eventPools.end()) {
			std::shared_ptr<Pool_<T>> pool(new Pool_<T>());
			eventPools.insert(std::make_pair(std::type_index(typeid(T)), pool));
		}

		return std::static_pointer_cast<Pool_<T>>(
				eventPools[std::type_index(typeid(T))])->getData();
	}


}
