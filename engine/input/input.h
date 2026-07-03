#pragma once
#include <SDL2/SDL.h>
#include <unordered_map>

namespace MoLin {

class Input {
public:
    static Input& Instance() {
        static Input instance;
        return instance;
    }

    void Update(); // 每帧调用
    bool IsKeyDown(SDL_Scancode key) const;
    bool IsKeyPressed(SDL_Scancode key) const; // 按下瞬间
    bool IsKeyReleased(SDL_Scancode key) const;

    void ProcessEvent(const SDL_Event& event);

private:
    Input() = default;
    const Uint8* m_KeyState = nullptr;
    std::unordered_map<SDL_Scancode, bool> m_PrevKeyState;
    std::unordered_map<SDL_Scancode, bool> m_CurrKeyState;
};

} // namespace MoLin
