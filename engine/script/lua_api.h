#pragma once

extern "C" {
#include <lua.h>
}

// Lua API 函数声明
int Lua_UseItem(lua_State* L);
int Lua_GetPlayerInfo(lua_State* L);
int Lua_SetPlayerPosition(lua_State* L);
int Lua_LoadMap(lua_State* L);
int Lua_GetInventory(lua_State* L);

// 注册所有 API
void RegisterLuaAPI(lua_State* L);
