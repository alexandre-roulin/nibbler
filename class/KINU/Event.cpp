#include "Event.hpp"
#include "World.hpp"

namespace KINU
{

BaseEvent::Id BaseEvent::nextId = 0;

void EventManager::destroyEvents()
{
    for (auto &it : eventPools) {
        auto pool = it.second;
        pool->clear();
    }
}

}
