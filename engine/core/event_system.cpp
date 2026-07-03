#include "event_system.h"

namespace MoLin {

void EventSystem::Register(const std::string& eventName, EventCallback callback) {
    m_Events[eventName].push_back(callback);
}

void EventSystem::Trigger(const std::string& eventName) {
    auto it = m_Events.find(eventName);
    if (it != m_Events.end()) {
        for (auto& cb : it->second) {
            cb();
        }
    }
}

void EventSystem::Clear() {
    m_Events.clear();
}

} // namespace MoLin
