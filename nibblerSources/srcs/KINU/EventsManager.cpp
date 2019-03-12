#include "EventsManager.hpp"

namespace KINU {

	BaseEvent::Id BaseEvent::nextId = 0;

	void EventsManager::destroyEvents() {
		std::lock_guard<std::mutex> guard(mutex_);
		for (auto &it : eventPools) {
			auto pool = it.second;
			pool->clear();
		}
	}
}
