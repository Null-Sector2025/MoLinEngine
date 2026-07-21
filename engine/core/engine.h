#pragma once
#include <SDL2/SDL.h>
#include <functional>
#include <string>

namespace MoLin {

struct WindowConfig {
    std::string title = "MoLinEngine";
    int width = 1280;
    int height = 720;
    bool fullscreen = false;
    bool vsync = true;
    bool resizable = false;
};

class Engine {
public:
    using UpdateCallback = std::function<void(float delta)>;
    using RenderCallback = std::function<void()>;
    using EventCallback = std::function<void(const SDL_Event&)>;

    Engine();
    ~Engine();

    bool Initialize(const WindowConfig& config = WindowConfig());
    void Run();
    void Shutdown();
    void Quit();

    void SetUpdateCallback(UpdateCallback cb);
    void SetRenderCallback(RenderCallback cb);
    void SetEventCallback(EventCallback cb);

    SDL_Window* GetWindow() const { return m_window; }
    SDL_Renderer* GetRenderer() const { return m_renderer; }
    float GetFPS() const { return m_fps; }
    float GetDeltaTime() const { return m_deltaTime; }

private:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    UpdateCallback m_updateCallback;
    RenderCallback m_renderCallback;
    EventCallback m_eventCallback;
    bool m_running = false;
    float m_fps = 0.0f;
    float m_deltaTime = 0.0f;
};

} // namespace MoLin