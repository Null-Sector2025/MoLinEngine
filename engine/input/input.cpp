#include "input.h"

namespace MoLin {

void Input::Update() {
    m_PrevKeyState = m_CurrKeyState;
    m_KeyState = SDL_GetKeyboardState(nullptr);
    // 填充当前帧按键状态
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
        m_CurrKeyState[static_cast<SDL_Scancode>(i)] = m_KeyState[i];
    }
}

bool Input::IsKeyDown(SDL_Scancode key) const {
    return m_KeyState[key];
}

bool Input::IsKeyPressed(SDL_Scancode key) const {
    return m_CurrKeyState.count(key) && m_CurrKeyState.at(key) &&
           (!m_PrevKeyState.count(key) || !m_PrevKeyState.at(key));
}

bool Input::IsKeyReleased(SDL_Scancode key) const {
    return m_PrevKeyState.count(key) && m_PrevKeyState.at(key) &&
           (!m_CurrKeyState.count(key) || !m_CurrKeyState.at(key));
}

void Input::ProcessEvent(const SDL_Event& event) {
    // 可处理手柄等，这里留空
}

} // namespace MoLin
