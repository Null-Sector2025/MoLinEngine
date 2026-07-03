#pragma once
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

namespace MoLin::RPG {

class Event {
public:
    std::string name;
    std::function<bool()> condition; // 返回 true 触发
    std::function<void()> action;
};

class EventManager {
public:
    void RegisterEvent(std::shared_ptr<Event> event);
    void Update();
    void TriggerEvent(const std::string& name); // 直接触发

private:
    std::vector<std::shared_ptr<Event>> m_Events;
};

} // namespace MoLin::RPG
