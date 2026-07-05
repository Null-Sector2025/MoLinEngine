#include "../core/engine.h"
#include "../debug/imgui/imgui.h"
#include "../debug/imgui/imgui_impl_sdl2.h"
#include "../debug/imgui/imgui_impl_sdlrenderer2.h"
#include <iostream>
#include <vector>

static bool showMapEditor = true;
static int mapWidth = 20, mapHeight = 15, tileSize = 32;
static std::vector<std::vector<int>> tiles;

void InitDefaultMap() {
    tiles.resize(mapHeight, std::vector<int>(mapWidth, 0));
}

int main(int argc, char* argv[]) {
    std::cout << "墨麟编辑器 v1.0 启动..." << std::endl;

    MoLin::WindowConfig config;
    config.title = "墨麟编辑器 - MoLin Editor";
    config.width = 1280;
    config.height = 720;

    MoLin::Engine engine;
    if (!engine.Initialize(config)) {
        std::cerr << "初始化失败！" << std::endl;
        return -1;
    }

    // 初始化 ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(engine.GetWindow(), engine.GetRenderer());
    ImGui_ImplSDLRenderer2_Init(engine.GetRenderer());

    InitDefaultMap();

    engine.SetEventCallback([&](const SDL_Event& event) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) engine.Quit();
    });

    engine.SetUpdateCallback([&](float dt) {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // 菜单
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("文件")) {
                if (ImGui::MenuItem("退出")) engine.Quit();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("视图")) {
                ImGui::MenuItem("地图编辑器", nullptr, &showMapEditor);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // 地图编辑窗口
        if (showMapEditor) {
            ImGui::Begin("地图编辑器", &showMapEditor);
            ImGui::InputInt("宽度", &mapWidth);
            ImGui::InputInt("高度", &mapHeight);
            if (ImGui::Button("新建地图")) {
                tiles.clear();
                InitDefaultMap();
            }
            ImGui::Separator();
            // 绘制瓦片网格（简化：用按钮代替）
            for (int y = 0; y < mapHeight; y++) {
                for (int x = 0; x < mapWidth; x++) {
                    char label[32];
                    sprintf(label, "%d", tiles[y][x]);
                    if (ImGui::Button(label, ImVec2(tileSize, tileSize))) {
                        tiles[y][x] = (tiles[y][x] + 1) % 3; // 循环切换
                    }
                    if (x < mapWidth - 1) ImGui::SameLine();
                }
            }
            ImGui::End();
        }
    });

    engine.SetRenderCallback([&](SDL_Renderer* renderer) {
        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    });

    engine.Run();

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    engine.Shutdown();
    return 0;
}
