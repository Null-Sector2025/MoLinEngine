#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace MoLin {

class EventSystem {
public:
    using EventCallback = std::function<void()>;

    static EventSystem& Instance() {
        static EventSystem instance;
        return instance;
    }

    void Register(const std::string& eventName, EventCallback callback);
    void Trigger(const std::string& eventName);
    void Clear();

private:
    EventSystem() = default;
    std::unordered_map<std::string, std::vector<EventCallback>> m_Events;
};

} // namespace MoLin
