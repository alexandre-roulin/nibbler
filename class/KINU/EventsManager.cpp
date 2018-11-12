#include "EventsManager.hpp"

namespace KINU {
	EventsManager::EventsManager(World &world)
			: world_(world) {
	}


	void EventsManager::destroyEvents() {
		for (auto &eventPool : eventPools) {
			eventPool.second->clean();
		}
	}
}