#include "script/lua_api.h"
#include "game/game_state.h"
#include "rpg/character.h"
#include "rpg/inventory.h"
#include <iostream>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

using namespace MoLin;

// === Lua API 函数实现 ===

int Lua_UseItem(lua_State* L) {
    const char* itemId = luaL_checkstring(L, 1);
    int count = luaL_optinteger(L, 2, 1);
    
    auto& state = GameState::Instance();
    auto& player = state.GetPlayer();
    auto* inv = player.GetInventory();
    if (inv && inv->HasItem(itemId, count)) {
        inv->RemoveItem(itemId, count);
        lua_pushboolean(L, 1);
    } else {
        lua_pushboolean(L, 0);
    }
    return 1;
}

int Lua_GetPlayerInfo(lua_State* L) {
    auto& state = GameState::Instance();
    auto& player = state.GetPlayer();
    lua_createtable(L, 0, 5);
    
    lua_pushstring(L, "level");
    lua_pushinteger(L, player.GetLevel());
    lua_settable(L, -3);
    
    lua_pushstring(L, "hp");
    lua_pushinteger(L, player.GetHp());
    lua_settable(L, -3);
    
    lua_pushstring(L, "maxHp");
    lua_pushinteger(L, player.GetMaxHp());
    lua_settable(L, -3);
    
    lua_pushstring(L, "atk");
    lua_pushinteger(L, player.GetAttack());
    lua_settable(L, -3);
    
    lua_pushstring(L, "def");
    lua_pushinteger(L, player.GetDefense());
    lua_settable(L, -3);
    
    return 1;
}

int Lua_SetPlayerPosition(lua_State* L) {
    float x = static_cast<float>(luaL_checknumber(L, 1));
    float y = static_cast<float>(luaL_checknumber(L, 2));
    auto& state = GameState::Instance();
    auto& player = state.GetPlayer();
    player.SetPosition(x, y);
    return 0;
}

int Lua_LoadMap(lua_State* L) {
    const char* mapName = luaL_checkstring(L, 1);
    std::cout << "加载地图: " << mapName << std::endl;
    return 0;
}

int Lua_GetInventory(lua_State* L) {
    auto& state = GameState::Instance();
    auto& player = state.GetPlayer();
    auto* inv = player.GetInventory();
    if (!inv) {
        lua_newtable(L);
        return 1;
    }
    lua_newtable(L);
    int index = 1;
    for (const auto& pair : inv->GetItems()) {
        lua_pushinteger(L, index++);
        lua_createtable(L, 0, 2);
        lua_pushstring(L, "id");
        lua_pushstring(L, pair.first.c_str());
        lua_settable(L, -3);
        lua_pushstring(L, "count");
        lua_pushinteger(L, pair.second);
        lua_settable(L, -3);
        lua_settable(L, -3);
    }
    return 1;
}

// 注册所有 Lua API
void RegisterLuaAPI(lua_State* L) {
    lua_register(L, "use_item", Lua_UseItem);
    lua_register(L, "get_player_info", Lua_GetPlayerInfo);
    lua_register(L, "set_player_position", Lua_SetPlayerPosition);
    lua_register(L, "load_map", Lua_LoadMap);
    lua_register(L, "get_inventory", Lua_GetInventory);
}
