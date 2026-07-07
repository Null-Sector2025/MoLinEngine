#include "timer.h"

namespace MoLin {

Timer::Timer(float interval, bool repeat, std::function<void()> cb) : m_Interval(interval), m_Repeat(repeat), m_Callback(cb) {}
void Timer::Start() { m_Running = true; m_Elapsed = 0.0f; m_Fired = false; }
void Timer::Stop() { m_Running = false; }
void Timer::Update(float delta) {
    if (!m_Running || m_Fired) return;
    m_Elapsed += delta;
    if (m_Elapsed >= m_Interval) {
        m_Fired = true;
        if (m_Callback) m_Callback();
        if (m_Repeat) { m_Elapsed -= m_Interval; m_Fired = false; }
        else m_Running = false;
    }
}

} // namespace MoLin
