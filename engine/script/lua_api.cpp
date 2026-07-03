#include "lua_api.h"
#include "../game/game_state.h"
#include "../rpg/inventory.h"
#include "../rpg/quest.h"
#include "../rpg/battle.h"
#include "../rpg/event_system.h"
#include "../audio/audio_player.h"
#include "../resource/resource_manager.h"
#include <iostream>
#include <memory>

namespace MoLin {

// ---- 全局引擎组件（游戏开发者需在初始化时设置）----
static std::shared_ptr<RPG::DialogueManager> g_DialogueMgr;
static std::shared_ptr<RPG::QuestManager> g_QuestMgr;
static std::shared_ptr<RPG::BattleSystem> g_BattleSystem;
static std::shared_ptr<RPG::EventManager> g_EventMgr;
static SDL_Renderer* g_Renderer = nullptr;

// 外部设置接口
void SetDialogueManager(std::shared_ptr<RPG::DialogueManager> mgr) { g_DialogueMgr = mgr; }
void SetQuestManager(std::shared_ptr<RPG::QuestManager> mgr) { g_QuestMgr = mgr; }
void SetBattleSystem(std::shared_ptr<RPG::BattleSystem> sys) { g_BattleSystem = sys; }
void SetEventManager(std::shared_ptr<RPG::EventManager> mgr) { g_EventMgr = mgr; }
void SetRenderer(SDL_Renderer* r) { g_Renderer = r; }

// ---- 注册所有 API ----
void LuaAPI::RegisterAll(LuaBind& lua) {
    lua_State* L = lua.GetState();

    lua_register(L, "Log",              Lua_Log);
    lua_register(L, "LoadScene",        Lua_LoadScene);
    lua_register(L, "GetCurrentScene",  Lua_GetCurrentScene);
    lua_register(L, "StartDialogue",    Lua_StartDialogue);
    lua_register(L, "ShowNextLine",     Lua_ShowNextLine);
    lua_register(L, "IsDialogueActive", Lua_IsDialogueActive);
    lua_register(L, "SaveGame",         Lua_SaveGame);
    lua_register(L, "LoadGame",         Lua_LoadGame);
    lua_register(L, "DeleteSave",       Lua_DeleteSave);
    lua_register(L, "GiveItem",         Lua_GiveItem);
    lua_register(L, "RemoveItem",       Lua_RemoveItem);
    lua_register(L, "GetItemCount",     Lua_GetItemCount);
    lua_register(L, "HasItem",          Lua_HasItem);
    lua_register(L, "StartQuest",       Lua_StartQuest);
    lua_register(L, "CompleteQuest",    Lua_CompleteQuest);
    lua_register(L, "FailQuest",        Lua_FailQuest);
    lua_register(L, "UpdateObjective",  Lua_UpdateObjective);
    lua_register(L, "GetPlayerHP",      Lua_GetPlayerHP);
    lua_register(L, "SetPlayerHP",      Lua_SetPlayerHP);
    lua_register(L, "GetPlayerMP",      Lua_GetPlayerMP);
    lua_register(L, "SetPlayerMP",      Lua_SetPlayerMP);
    lua_register(L, "GetPlayerLevel",   Lua_GetPlayerLevel);
    lua_register(L, "AddExp",           Lua_AddExp);
    lua_register(L, "StartBattle",      Lua_StartBattle);
    lua_register(L, "IsBattleOver",     Lua_IsBattleOver);
    lua_register(L, "TriggerEvent",     Lua_TriggerEvent);
    lua_register(L, "PlaySound",        Lua_PlaySound);
    lua_register(L, "PlayMusic",        Lua_PlayMusic);
    lua_register(L, "StopMusic",        Lua_StopMusic);
    lua_register(L, "SetFlag",          Lua_SetFlag);
    lua_register(L, "GetFlag",          Lua_GetFlag);
    lua_register(L, "HasFlag",          Lua_HasFlag);
}

// ==================== 基础 ====================
int LuaAPI::Lua_Log(lua_State* L) {
    const char* msg = lua_tostring(L, 1);
    if (msg) std::cout << "[Lua] " << msg << std::endl;
    return 0;
}

// ==================== 场景 ====================
int LuaAPI::Lua_LoadScene(lua_State* L) {
    const char* scene = lua_tostring(L, 1);
    if (scene) {
        auto& player = GameState::Instance().GetPlayer();
        player.sceneId = scene;
        std::cout << "[Lua] LoadScene: " << scene << std::endl;
    }
    return 0;
}
int LuaAPI::Lua_GetCurrentScene(lua_State* L) {
    lua_pushstring(L, GameState::Instance().GetPlayer().sceneId.c_str());
    return 1;
}

// ==================== 对话 ====================
int LuaAPI::Lua_StartDialogue(lua_State* L) {
    int n = lua_gettop(L);
    if (n < 2) return 0;
    std::vector<RPG::DialogueLine> lines;
    for (int i = 1; i <= n; i += 2) {
        if (i + 1 > n) break;
        const char* speaker = lua_tostring(L, i);
        const char* text    = lua_tostring(L, i + 1);
        if (speaker && text) lines.push_back({speaker, text});
    }
    if (g_DialogueMgr && !lines.empty()) {
        g_DialogueMgr->StartDialogue(lines);
    }
    return 0;
}
int LuaAPI::Lua_ShowNextLine(lua_State* L) {
    if (g_DialogueMgr) g_DialogueMgr->ShowNextLine();
    return 0;
}
int LuaAPI::Lua_IsDialogueActive(lua_State* L) {
    lua_pushboolean(L, g_DialogueMgr ? g_DialogueMgr->IsActive() : false);
    return 1;
}

// ==================== 存档 ====================
int LuaAPI::Lua_SaveGame(lua_State* L) {
    int slot = lua_tointeger(L, 1);
    if (slot <= 0) slot = 1;
    lua_pushboolean(L, GameState::Instance().Save(slot));
    return 1;
}
int LuaAPI::Lua_LoadGame(lua_State* L) {
    int slot = lua_tointeger(L, 1);
    if (slot <= 0) slot = 1;
    lua_pushboolean(L, GameState::Instance().Load(slot));
    return 1;
}
int LuaAPI::Lua_DeleteSave(lua_State* L) {
    int slot = lua_tointeger(L, 1);
    if (slot <= 0) slot = 1;
    lua_pushboolean(L, GameState::Instance().DeleteSave(slot));
    return 1;
}

// ==================== 物品 ====================
int LuaAPI::Lua_GiveItem(lua_State* L) {
    const char* id = lua_tostring(L, 1);
    int count = lua_tointeger(L, 2);
    if (count <= 0) count = 1;
    if (id) {
        auto& inv = GameState::Instance().GetPlayer().inventory;
        inv[id] += count;
        std::cout << "[Lua] GiveItem: " << id << " x" << count << std::endl;
    }
    return 0;
}
int LuaAPI::Lua_RemoveItem(lua_State* L) {
    const char* id = lua_tostring(L, 1);
    int count = lua_tointeger(L, 2);
    if (count <= 0) count = 1;
    auto& inv = GameState::Instance().GetPlayer().inventory;
    auto it = inv.find(id);
    if (it != inv.end()) {
        it->second -= count;
        if (it->second <= 0) inv.erase(it);
        lua_pushboolean(L, true);
    } else {
        lua_pushboolean(L, false);
    }
    return 1;
}
int LuaAPI::Lua_GetItemCount(lua_State* L) {
    const char* id = lua_tostring(L, 1);
    auto& inv = GameState::Instance().GetPlayer().inventory;
    auto it = inv.find(id);
    lua_pushinteger(L, it != inv.end() ? it->second : 0);
    return 1;
}
int LuaAPI::Lua_HasItem(lua_State* L) {
    const char* id = lua_tostring(L, 1);
    int count = lua_tointeger(L, 2);
    if (count <= 0) count = 1;
    auto& inv = GameState::Instance().GetPlayer().inventory;
    auto it = inv.find(id);
    lua_pushboolean(L, it != inv.end() && it->second >= count);
    return 1;
}

// ==================== 任务 ====================
int LuaAPI::Lua_StartQuest(lua_State* L) {
    const char* id = lua_tostring(L, 1);
    if (id && g_QuestMgr) {
        g_QuestMgr->StartQuest(id);
        GameState::Instance().GetPlayer().quests[id] = 1;
    }
    return 0;
}
int LuaAPI::Lua_CompleteQuest(lua_State* L) {
    const char* id = lua_tostring(L, 1);
    if (id) GameState::Instance().GetPlayer().quests[id] = 2;
    return 0;
}
int LuaAPI::Lua_FailQuest(lua_State* L) {
    const char* id = lua_tostring(L, 1);
    if (id) GameState::Instance().GetPlayer().quests[id] = -1;
    return 0;
}
int LuaAPI::Lua_UpdateObjective(lua_State* L) {
    const char* id = lua_tostring(L, 1);
    int idx = lua_tointeger(L, 2);
    int prog = lua_tointeger(L, 3);
    if (id && g_QuestMgr) g_QuestMgr->UpdateObjective(id, idx, prog);
    return 0;
}

// ==================== 角色属性 ====================
int LuaAPI::Lua_GetPlayerHP(lua_State* L) {
    lua_pushinteger(L, GameState::Instance().GetPlayer().hp);
    return 1;
}
int LuaAPI::Lua_SetPlayerHP(lua_State* L) {
    int val = lua_tointeger(L, 1);
    GameState::Instance().GetPlayer().hp = val;
    return 0;
}
int LuaAPI::Lua_GetPlayerMP(lua_State* L) {
    lua_pushinteger(L, GameState::Instance().GetPlayer().mp);
    return 1;
}
int LuaAPI::Lua_SetPlayerMP(lua_State* L) {
    int val = lua_tointeger(L, 1);
    GameState::Instance().GetPlayer().mp = val;
    return 0;
}
int LuaAPI::Lua_GetPlayerLevel(lua_State* L) {
    lua_pushinteger(L, GameState::Instance().GetPlayer().level);
    return 1;
}
int LuaAPI::Lua_AddExp(lua_State* L) {
    int exp = lua_tointeger(L, 1);
    auto& p = GameState::Instance().GetPlayer();
    p.exp += exp;
    if (p.exp >= p.level * 100) {
        p.exp -= p.level * 100;
        p.level++;
        std::cout << "[Lua] Level up! Now level " << p.level << std::endl;
    }
    lua_pushinteger(L, p.level);
    return 1;
}

// ==================== 战斗 ====================
int LuaAPI::Lua_StartBattle(lua_State* L) {
    if (g_BattleSystem) {
        // 简化：创建空的战斗实体列表（实际由 C++ 侧填充）
        std::vector<std::shared_ptr<RPG::BattleEntity>> allies, enemies;
        g_BattleSystem->StartBattle(allies, enemies);
        lua_pushboolean(L, true);
    } else {
        lua_pushboolean(L, false);
    }
    return 1;
}
int LuaAPI::Lua_IsBattleOver(lua_State* L) {
    lua_pushboolean(L, g_BattleSystem ? g_BattleSystem->IsBattleOver() : true);
    return 1;
}

// ==================== 事件 ====================
int LuaAPI::Lua_TriggerEvent(lua_State* L) {
    const char* name = lua_tostring(L, 1);
    if (name && g_EventMgr) g_EventMgr->TriggerEvent(name);
    return 0;
}

// ==================== 音频 ====================
int LuaAPI::Lua_PlaySound(lua_State* L) {
    const char* path = lua_tostring(L, 1);
    if (path && g_Renderer) {
        auto* chunk = ResourceManager::Instance().LoadSound(path);
        AudioPlayer::Instance().PlaySound(chunk);
    }
    return 0;
}
int LuaAPI::Lua_PlayMusic(lua_State* L) {
    const char* path = lua_tostring(L, 1);
    if (path) {
        auto* music = ResourceManager::Instance().LoadMusic(path);
        AudioPlayer::Instance().PlayMusic(music);
    }
    return 0;
}
int LuaAPI::Lua_StopMusic(lua_State* L) {
    AudioPlayer::Instance().StopMusic();
    return 0;
}

// ==================== 全局标记 ====================
int LuaAPI::Lua_SetFlag(lua_State* L) {
    const char* key = lua_tostring(L, 1);
    const char* val = lua_tostring(L, 2);
    if (key && val) GameState::Instance().GetPlayer().flags[key] = val;
    return 0;
}
int LuaAPI::Lua_GetFlag(lua_State* L) {
    const char* key = lua_tostring(L, 1);
    auto& flags = GameState::Instance().GetPlayer().flags;
    auto it = flags.find(key);
    lua_pushstring(L, it != flags.end() ? it->second.c_str() : "");
    return 1;
}
int LuaAPI::Lua_HasFlag(lua_State* L) {
    const char* key = lua_tostring(L, 1);
    lua_pushboolean(L, GameState::Instance().GetPlayer().flags.find(key) != GameState::Instance().GetPlayer().flags.end());
    return 1;
}

} // namespace MoLin
