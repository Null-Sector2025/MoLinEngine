#include "event_system.h"
#include <iostream>
#include <algorithm>

namespace MoLin::RPG {

void EventManager::RegisterEvent(std::shared_ptr<Event> event) {
    m_Events.push_back(event);
}

void EventManager::Update(float delta) {
    for (auto& ev : m_Events) {
        if (!ev->active) continue;
        ev->elapsed += delta;
        if (ev->elapsed < ev->delay) continue;

        bool triggered = ev->condition ? ev->condition() : true;
        if (triggered) {
            std::cout << "[Event] " << ev->name << " triggered" << std::endl;
            if (ev->action) ev->action();
            if (ev->oneShot) ev->active = false;
            ev->elapsed = 0.0f; // 重置计时器（如重复触发）
        }
    }
}

void EventManager::TriggerEvent(const std::string& name) {
    for (auto& ev : m_Events) {
        if (ev->name == name && ev->action) {
            ev->action();
            if (ev->oneShot) ev->active = false;
            return;
        }
    }
}

void EventManager::RemoveEvent(const std::string& name) {
    m_Events.erase(std::remove_if(m_Events.begin(), m_Events.end(),
        [&](auto& e) { return e->name == name; }), m_Events.end());
}

void EventManager::Clear() {
    m_Events.clear();
}

} // namespace MoLin::RPG
