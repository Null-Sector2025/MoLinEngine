#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace MoLin::RPG {

struct Event {
    std::string name;
    bool active = true;
    bool oneShot = false;       // 触发一次后失效
    float delay = 0.0f;         // 延迟秒数
    float elapsed = 0.0f;
    std::function<bool()> condition; // 返回true触发
    std::function<void()> action;
};

class EventManager {
public:
    void RegisterEvent(std::shared_ptr<Event> event);
    void Update(float delta);
    void TriggerEvent(const std::string& name);
    void RemoveEvent(const std::string& name);
    void Clear();

private:
    std::vector<std::shared_ptr<Event>> m_Events;
};

} // namespace MoLin::RPG
