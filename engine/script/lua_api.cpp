#include "lua_api.h"
#include <iostream>

namespace MoLin {

void LuaAPI::RegisterAll(LuaBind& lua) {
    lua_State* L = lua.GetState();
    lua_register(L, "Log", Lua_Log);
    lua_register(L, "LoadScene", Lua_LoadScene);
    lua_register(L, "StartDialogue", Lua_StartDialogue);
}

int LuaAPI::Lua_Log(lua_State* L) {
    const char* msg = lua_tostring(L, 1);
    if (msg) std::cout << "[Lua] " << msg << std::endl;
    return 0;
}

int LuaAPI::Lua_LoadScene(lua_State* L) {
    const char* scene = lua_tostring(L, 1);
    if (scene) std::cout << "[Lua] LoadScene: " << scene << std::endl;
    // 实际场景切换由 C++ 侧处理，此处仅演示
    return 0;
}

int LuaAPI::Lua_StartDialogue(lua_State* L) {
    // 参数：说话人, 内容
    const char* speaker = lua_tostring(L, 1);
    const char* text = lua_tostring(L, 2);
    if (speaker && text) std::cout << "[Lua] Dialogue: " << speaker << ": " << text << std::endl;
    return 0;
}

} // namespace MoLin
