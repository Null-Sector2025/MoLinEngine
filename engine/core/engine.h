#pragma once
#include <string>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

namespace MoLin {

struct WindowConfig {
    std::string title = "墨麟引擎";
    int width = 1280;
    int height = 720;
    bool fullscreen = false;
    bool vsync = true;
    bool resizable = false;
};

class Engine {
public:
    using UpdateCallback = std::function<void(float)>;
    using RenderCallback = std::function<void(SDL_Renderer*)>;
    using EventCallback = std::function<void(const SDL_Event&)>;

    Engine();
    ~Engine();

    bool Initialize(const WindowConfig& config = WindowConfig());
    void Run();
    void Shutdown();
    void Quit();

    void SetUpdateCallback(UpdateCallback cb) { m_UpdateCB = cb; }
    void SetRenderCallback(RenderCallback cb) { m_RenderCB = cb; }
    void SetEventCallback(EventCallback cb) { m_EventCB = cb; }

    SDL_Window* GetWindow() const { return m_Window; }
    SDL_Renderer* GetRenderer() const { return m_Renderer; }

    float GetFPS() const { return m_FPS; }
    float GetDeltaTime() const { return m_DeltaTime; }
    int GetWidth() const { return m_Config.width; }
    int GetHeight() const { return m_Config.height; }

    void SetClearColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

private:
    WindowConfig m_Config;
    SDL_Window* m_Window = nullptr;
    SDL_Renderer* m_Renderer = nullptr;
    bool m_Running = false;
    float m_FPS = 0.0f;
    float m_DeltaTime = 0.0f;
    SDL_Color m_ClearColor = {0, 0, 0, 255};

    UpdateCallback m_UpdateCB;
    RenderCallback m_RenderCB;
    EventCallback m_EventCB;

    void HandleEvents();
    void UpdateFPS(float delta);
};

} // namespace MoLin
