#include "EventsManager.hpp"

namespace KINU {

	BaseEvent::Id BaseEvent::nextId = 0;

	void EventsManager::destroyEvents() {
		for (auto &it : eventPools) {
			auto pool = it.second;
			pool->clear();
		}
	}
}
