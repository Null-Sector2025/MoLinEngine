#define SDL_MAIN_HANDLED
#include "core/engine.h"
#include "graphics/sprite.h"
#include "../scene/scene.h"
#include "audio/audio_manager.h"
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
    std::cout << "墨麟引擎 v1.2.0 启动中..." << std::endl;

    MoLin::Engine engine;
    MoLin::WindowConfig config;
    config.title = "墨麟引擎 - MoLin Engine";
    config.width = 1280;
    config.height = 720;
    config.vsync = true;

    if (!engine.Initialize(config)) {
        std::cerr << "引擎初始化失败！" << std::endl;
        return -1;
    }

    auto scene = std::make_unique<MoLin::Scene>();

    engine.SetUpdateCallback([&](float dt) {
        scene->Update(dt);
    });

    engine.SetRenderCallback([&](SDL_Renderer* renderer) {
        scene->Render(renderer);
    });

    engine.Run();
    return 0;
}
