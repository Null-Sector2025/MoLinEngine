#include "engine.h"
#include <iostream>

namespace MoLin {

Engine::Engine() {}
Engine::~Engine() { Shutdown(); }

bool Engine::Initialize(const WindowConfig& config) {
    m_Config = config;

    Uint32 sdlFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS;
    if (SDL_Init(sdlFlags) < 0) {
        std::cerr << "[Engine] SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    Uint32 windowFlags = SDL_WINDOW_SHOWN;
    if (m_Config.fullscreen) windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    if (m_Config.resizable) windowFlags |= SDL_WINDOW_RESIZABLE;

    m_Window = SDL_CreateWindow(m_Config.title.c_str(),
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                m_Config.width, m_Config.height, windowFlags);
    if (!m_Window) {
        std::cerr << "[Engine] SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return false;
    }

    Uint32 renderFlags = SDL_RENDERER_ACCELERATED;
    if (m_Config.vsync) renderFlags |= SDL_RENDERER_PRESENTVSYNC;

    m_Renderer = SDL_CreateRenderer(m_Window, -1, renderFlags);
    if (!m_Renderer) {
        std::cerr << "[Engine] SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
        std::cerr << "[Engine] IMG_Init failed: " << IMG_GetError() << std::endl;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "[Engine] Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
    }
    Mix_AllocateChannels(16);

    if (TTF_Init() == -1) {
        std::cerr << "[Engine] TTF_Init failed: " << TTF_GetError() << std::endl;
    }

    m_Running = true;
    std::cout << "[Engine] 墨麟引擎初始化完成 " << m_Config.width << "x" << m_Config.height << std::endl;
    return true;
}

void Engine::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_Running = false;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            // 默认ESC不退出，交给游戏逻辑
        }
        if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                m_Config.width = event.window.data1;
                m_Config.height = event.window.data2;
            }
        }
        if (m_EventCB) m_EventCB(event);
    }
}

void Engine::UpdateFPS(float delta) {
    static int frameCount = 0;
    static float fpsTimer = 0.0f;
    frameCount++;
    fpsTimer += delta;
    if (fpsTimer >= 1.0f) {
        m_FPS = frameCount / fpsTimer;
        frameCount = 0;
        fpsTimer = 0.0f;
    }
}

void Engine::Run() {
    Uint32 lastTime = SDL_GetTicks();
    while (m_Running) {
        Uint32 current = SDL_GetTicks();
        m_DeltaTime = (current - lastTime) / 1000.0f;
        lastTime = current;

        if (m_DeltaTime > 0.1f) m_DeltaTime = 0.1f; // 防止帧率过低导致物理异常

        HandleEvents();
        UpdateFPS(m_DeltaTime);

        if (m_UpdateCB) m_UpdateCB(m_DeltaTime);

        SDL_SetRenderDrawColor(m_Renderer, m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
        SDL_RenderClear(m_Renderer);

        if (m_RenderCB) m_RenderCB(m_Renderer);

        SDL_RenderPresent(m_Renderer);
    }
}

void Engine::Quit() {
    m_Running = false;
}

void Engine::SetClearColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    m_ClearColor = {r, g, b, a};
}

void Engine::Shutdown() {
    if (m_Renderer) { SDL_DestroyRenderer(m_Renderer); m_Renderer = nullptr; }
    if (m_Window) { SDL_DestroyWindow(m_Window); m_Window = nullptr; }
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    std::cout << "[Engine] 墨麟引擎已关闭" << std::endl;
}

} // namespace MoLin
