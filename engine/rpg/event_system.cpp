#include "event_system.h"
#include <iostream>

namespace MoLin::RPG {

void EventManager::RegisterEvent(std::shared_ptr<Event> event) {
    m_Events.push_back(event);
}

void EventManager::Update() {
    for (auto& ev : m_Events) {
        if (ev->condition && ev->condition()) {
            std::cout << "[Event] Triggered: " << ev->name << std::endl;
            if (ev->action) ev->action();
        }
    }
}

void EventManager::TriggerEvent(const std::string& name) {
    for (auto& ev : m_Events) {
        if (ev->name == name && ev->action) {
            ev->action();
            return;
        }
    }
}

} // namespace MoLin::RPG
