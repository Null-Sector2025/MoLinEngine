#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <iostream>

namespace MoLin {

template<typename T>
class StateMachine {
public:
    void AddState(const T& state, std::function<void()> onEnter = nullptr,
                  std::function<void(float)> onUpdate = nullptr,
                  std::function<void()> onExit = nullptr) {
        m_States[state] = {onEnter, onUpdate, onExit};
    }

    void ChangeState(const T& newState) {
        auto it = m_States.find(m_CurrentState);
        if (it != m_States.end() && it->second.onExit) it->second.onExit();

        m_CurrentState = newState;
        auto newIt = m_States.find(newState);
        if (newIt != m_States.end() && newIt->second.onEnter) newIt->second.onEnter();
    }

    void Update(float delta) {
        auto it = m_States.find(m_CurrentState);
        if (it != m_States.end() && it->second.onUpdate) it->second.onUpdate(delta);
    }

    T GetCurrentState() const { return m_CurrentState; }

private:
    struct StateData {
        std::function<void()> onEnter;
        std::function<void(float)> onUpdate;
        std::function<void()> onExit;
    };
    T m_CurrentState;
    std::unordered_map<T, StateData> m_States;
};

} // namespace MoLin
