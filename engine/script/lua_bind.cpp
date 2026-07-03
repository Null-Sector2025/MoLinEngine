#include "lua_bind.h"
#include <iostream>

namespace MoLin {

// 示例：注册一个全局函数 Log
static int Lua_Log(lua_State* L) {
    const char* msg = lua_tostring(L, 1);
    if (msg) std::cout << "[Lua] " << msg << std::endl;
    return 0;
}

LuaBind::LuaBind() {
    L = luaL_newstate();
    luaL_openlibs(L);
    RegisterCoreFunctions();
}

LuaBind::~LuaBind() {
    if (L) lua_close(L);
}

void LuaBind::RegisterCoreFunctions() {
    lua_register(L, "Log", Lua_Log);
    // 后续注册更多引擎 API
}

bool LuaBind::DoFile(const std::string& filename) {
    if (luaL_dofile(L, filename.c_str()) != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        return false;
    }
    return true;
}

bool LuaBind::DoString(const std::string& code) {
    if (luaL_dostring(L, code.c_str()) != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        return false;
    }
    return true;
}

} // namespace MoLin
