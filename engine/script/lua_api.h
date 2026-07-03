#pragma once
#include "lua_bind.h"

namespace MoLin {

// 扩展 LuaBind，注册更多游戏相关 API
class LuaAPI {
public:
    static void RegisterAll(LuaBind& lua);
private:
    static int Lua_Log(lua_State* L);
    static int Lua_LoadScene(lua_State* L);
    static int Lua_StartDialogue(lua_State* L);
    // 更多...
};

} // namespace MoLin
