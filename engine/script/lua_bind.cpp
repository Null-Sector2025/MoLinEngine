#include "script/lua_bind.h"
#include "script/lua_api.h"
#include <iostream>

namespace MoLin {
namespace Script {

// 日志函数（供 Lua 调用）
static int Lua_Log(lua_State* L) {
    const char* msg = luaL_checkstring(L, 1);
    std::cout << "[Lua] " << msg << std::endl;
    return 0;
}

LuaBind::LuaBind() {
    L = luaL_newstate();
    if (L) {
        luaL_openlibs(L);
        RegisterCoreFunctions();
        RegisterLuaAPI(L);
    }
}

LuaBind::~LuaBind() {
    if (L) {
        lua_close(L);
    }
}

void LuaBind::RegisterCoreFunctions() {
    lua_register(L, "Log", Lua_Log);
}

bool LuaBind::DoFile(const std::string& filename) {
    if (!L) return false;
    if (luaL_dofile(L, filename.c_str()) != LUA_OK) {
        const char* err = lua_tostring(L, -1);
        std::cerr << "Lua 加载文件失败: " << filename << " - " << err << std::endl;
        lua_pop(L, 1);
        return false;
    }
    return true;
}

bool LuaBind::DoString(const std::string& code) {
    if (!L) return false;
    if (luaL_dostring(L, code.c_str()) != LUA_OK) {
        const char* err = lua_tostring(L, -1);
        std::cerr << "Lua 执行代码失败: " << err << std::endl;
        lua_pop(L, 1);
        return false;
    }
    return true;
}

} // namespace Script
} // namespace MoLin
