#pragma once

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace MoLin {
namespace Script {

// Lua 绑定初始化
bool InitLua(lua_State* L);
void CloseLua(lua_State* L);

// 注册所有 C++ 函数到 Lua
void RegisterAllBindings(lua_State* L);

} // namespace Script
} // namespace MoLin
