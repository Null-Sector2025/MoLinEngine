#pragma once
#include <SDL2/SDL.h>
#include <functional>

namespace MoLin {

class Timer {
public:
    Timer(float intervalSeconds, bool repeat = false, std::function<void()> callback = nullptr);
    void Update(float delta);
    void Start();
    void Stop();
    void SetCallback(std::function<void()> cb) { m_Callback = cb; }
    bool HasFired() const { return m_Fired; }

private:
    float m_Interval;
    float m_Elapsed = 0.0f;
    bool m_Repeat;
    bool m_Running = false;
    bool m_Fired = false;
    std::function<void()> m_Callback;
};

} // namespace MoLin
