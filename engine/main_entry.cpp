#include <iostream>
#include "core/engine.h"
#include "game/game_state.h"

int main() {
    std::cout << "MoLinEngine 启动中..." << std::endl;
    
    MoLin::Engine engine;
    MoLin::WindowConfig config;
    config.title = "MoLinEngine Demo";
    config.width = 800;
    config.height = 600;
    config.vsync = true;
    
    if (!engine.Initialize(config)) {
        std::cerr << "引擎初始化失败！" << std::endl;
        return -1;
    }
    
    // 注册回调
    engine.SetUpdateCallback([](float dt) {
        // 更新逻辑
    });
    
    engine.SetRenderCallback([]() {
        // 渲染逻辑
    });
    
    std::cout << "引擎启动成功，进入主循环..." << std::endl;
    engine.Run();
    engine.Shutdown();
    
    return 0;
}
