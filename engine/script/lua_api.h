#pragma once
#include "lua_bind.h"

namespace MoLin {

class LuaAPI {
public:
    static void RegisterAll(LuaBind& lua);

private:
    // 基础日志
    static int Lua_Log(lua_State* L);

    // 场景管理
    static int Lua_LoadScene(lua_State* L);
    static int Lua_GetCurrentScene(lua_State* L);

    // 对话系统
    static int Lua_StartDialogue(lua_State* L);
    static int Lua_ShowNextLine(lua_State* L);
    static int Lua_IsDialogueActive(lua_State* L);

    // 存档系统
    static int Lua_SaveGame(lua_State* L);
    static int Lua_LoadGame(lua_State* L);
    static int Lua_DeleteSave(lua_State* L);

    // 物品与背包
    static int Lua_GiveItem(lua_State* L);
    static int Lua_RemoveItem(lua_State* L);
    static int Lua_GetItemCount(lua_State* L);
    static int Lua_HasItem(lua_State* L);

    // 任务系统
    static int Lua_StartQuest(lua_State* L);
    static int Lua_CompleteQuest(lua_State* L);
    static int Lua_FailQuest(lua_State* L);
    static int Lua_UpdateObjective(lua_State* L);

    // 角色属性
    static int Lua_GetPlayerHP(lua_State* L);
    static int Lua_SetPlayerHP(lua_State* L);
    static int Lua_GetPlayerMP(lua_State* L);
    static int Lua_SetPlayerMP(lua_State* L);
    static int Lua_GetPlayerLevel(lua_State* L);
    static int Lua_AddExp(lua_State* L);

    // 战斗系统
    static int Lua_StartBattle(lua_State* L);
    static int Lua_IsBattleOver(lua_State* L);

    // 事件系统
    static int Lua_TriggerEvent(lua_State* L);

    // 音频
    static int Lua_PlaySound(lua_State* L);
    static int Lua_PlayMusic(lua_State* L);
    static int Lua_StopMusic(lua_State* L);

    // 全局标记
    static int Lua_SetFlag(lua_State* L);
    static int Lua_GetFlag(lua_State* L);
    static int Lua_HasFlag(lua_State* L);
};

} // namespace MoLin
