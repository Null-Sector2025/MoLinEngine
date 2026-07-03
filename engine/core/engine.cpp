#include "../engine.h"
#include <iostream>

namespace MoLin {

Engine::Engine(const std::string& title, int width, int height)
    : m_Title(title), m_Width(width), m_Height(height) {}

Engine::~Engine() { Shutdown(); }

bool Engine::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL Init failed: " << SDL_GetError() << std::endl;
        return false;
    }
    m_Window = SDL_CreateWindow(m_Title.c_str(),
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                m_Width, m_Height,
                                SDL_WINDOW_SHOWN);
    if (!m_Window) return false;
    m_Renderer = SDL_CreateRenderer(m_Window, -1,
                                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_Renderer) return false;

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image init failed: " << IMG_GetError() << std::endl;
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer init failed: " << Mix_GetError() << std::endl;
        return false;
    }
    m_Running = true;
    return true;
}

void Engine::Run() {
    SDL_Event event;
    Uint32 lastTime = SDL_GetTicks();
    int frameCount = 0;
    float fpsTimer = 0.0f;

    while (m_Running) {
        Uint32 current = SDL_GetTicks();
        float delta = (current - lastTime) / 1000.0f;
        lastTime = current;

        fpsTimer += delta;
        frameCount++;
        if (fpsTimer >= 1.0f) {
            m_FPS = frameCount / fpsTimer;
            frameCount = 0;
            fpsTimer = 0.0f;
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) m_Running = false;
        }

        if (m_UpdateCB) m_UpdateCB(delta);

        SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
        SDL_RenderClear(m_Renderer);
        if (m_RenderCB) m_RenderCB(m_Renderer);
        SDL_RenderPresent(m_Renderer);
    }
}

void Engine::Shutdown() {
    if (m_Renderer) { SDL_DestroyRenderer(m_Renderer); m_Renderer = nullptr; }
    if (m_Window) { SDL_DestroyWindow(m_Window); m_Window = nullptr; }
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

} // namespace MoLin
