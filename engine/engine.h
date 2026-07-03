#pragma once
#include <string>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

namespace MoLin {

class Engine {
public:
    using UpdateCallback = std::function<void(float)>;
    using RenderCallback = std::function<void(SDL_Renderer*)>;

    Engine(const std::string& title, int width, int height);
    ~Engine();

    bool Initialize();
    void Run();
    void Shutdown();

    void SetUpdateCallback(UpdateCallback cb) { m_UpdateCB = cb; }
    void SetRenderCallback(RenderCallback cb) { m_RenderCB = cb; }

    SDL_Window* GetWindow() const { return m_Window; }
    SDL_Renderer* GetRenderer() const { return m_Renderer; }

    float GetFPS() const { return m_FPS; }

private:
    std::string m_Title;
    int m_Width, m_Height;
    SDL_Window* m_Window = nullptr;
    SDL_Renderer* m_Renderer = nullptr;
    bool m_Running = false;
    float m_FPS = 0.0f;

    UpdateCallback m_UpdateCB;
    RenderCallback m_RenderCB;
};

} // namespace MoLin
