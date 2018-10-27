#include "EventManager.hpp"

namespace KNU {
	void EventManager::destroyEvents() {
		for (auto &pool : eventPools) {
			pool.second->clean();
		}
	}

	EventManager::EventManager(World &world) : world(world) {}
}