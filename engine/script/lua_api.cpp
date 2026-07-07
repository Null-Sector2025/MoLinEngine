#include "lua_api.h"
#include "game/game_state.h"
#include "rpg/dialogue.h"
#include "rpg/quest.h"
#include "rpg/battle.h"
#include "rpg/event_system.h"
#include "rpg/inventory.h"
#include "audio/audio_manager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace MoLin {

static std::shared_ptr<RPG::DialogueManager> g_DialogueMgr;
static std::shared_ptr<RPG::QuestManager> g_QuestMgr;
static std::shared_ptr<RPG::BattleSystem> g_BattleSystem;
static std::shared_ptr<RPG::EventManager> g_EventMgr;
static SDL_Renderer* g_Renderer = nullptr;

void LuaAPI::SetDialogueManager(std::shared_ptr<RPG::DialogueManager> mgr) { g_DialogueMgr = mgr; }
void LuaAPI::SetQuestManager(std::shared_ptr<RPG::QuestManager> mgr) { g_QuestMgr = mgr; }
void LuaAPI::SetBattleSystem(std::shared_ptr<RPG::BattleSystem> sys) { g_BattleSystem = sys; }
void LuaAPI::SetEventManager(std::shared_ptr<RPG::EventManager> mgr) { g_EventMgr = mgr; }
void LuaAPI::SetRenderer(SDL_Renderer* r) { g_Renderer = r; }

void LuaAPI::RegisterAll(LuaBind& lua) {
    lua_State* L = lua.GetState();
    lua_register(L, "Log", Lua_Log);
    lua_register(L, "Sleep", Lua_Sleep);
    lua_register(L, "Random", Lua_Random);
    lua_register(L, "RandomRange", Lua_RandomRange);
    lua_register(L, "LoadScene", Lua_LoadScene);
    lua_register(L, "GetCurrentScene", Lua_GetCurrentScene);
    lua_register(L, "SaveGame", Lua_SaveGame);
    lua_register(L, "LoadGame", Lua_LoadGame);
    lua_register(L, "DeleteSave", Lua_DeleteSave);
    lua_register(L, "GetSaveSlots", Lua_GetSaveSlots);
    lua_register(L, "StartDialogue", Lua_StartDialogue);
    lua_register(L, "ShowNextLine", Lua_ShowNextLine);
    lua_register(L, "IsDialogueActive", Lua_IsDialogueActive);
    lua_register(L, "GiveItem", Lua_GiveItem);
    lua_register(L, "RemoveItem", Lua_RemoveItem);
    lua_register(L, "GetItemCount", Lua_GetItemCount);
    lua_register(L, "HasItem", Lua_HasItem);
    lua_register(L, "UseItem", Lua_UseItem);
    lua_register(L, "StartQuest", Lua_StartQuest);
    lua_register(L, "CompleteQuest", Lua_CompleteQuest);
    lua_register(L, "FailQuest", Lua_FailQuest);
    lua_register(L, "UpdateObjective", Lua_UpdateObjective);
    lua_register(L, "GetHP", Lua_GetHP);
    lua_register(L, "SetHP", Lua_SetHP);
    lua_register(L, "GetMP", Lua_GetMP);
    lua_register(L, "SetMP", Lua_SetMP);
    lua_register(L, "GetLevel", Lua_GetLevel);
    lua_register(L, "AddExp", Lua_AddExp);
    lua_register(L, "Heal", Lua_Heal);
    lua_register(L, "Damage", Lua_Damage);
    lua_register(L, "StartBattle", Lua_StartBattle);
    lua_register(L, "IsBattleOver", Lua_IsBattleOver);
    lua_register(L, "ExecuteBattleCommand", Lua_ExecuteBattleCommand);
    lua_register(L, "PlaySound", Lua_PlaySound);
    lua_register(L, "PlayMusic", Lua_PlayMusic);
    lua_register(L, "StopMusic", Lua_StopMusic);
    lua_register(L, "SetMusicVolume", Lua_SetMusicVolume);
    lua_register(L, "SetFlag", Lua_SetFlag);
    lua_register(L, "GetFlag", Lua_GetFlag);
    lua_register(L, "HasFlag", Lua_HasFlag);
    lua_register(L, "ClearFlag", Lua_ClearFlag);
    lua_register(L, "SetCameraPos", Lua_SetCameraPos);
    lua_register(L, "GetCameraPos", Lua_GetCameraPos);
    lua_register(L, "ShakeCamera", Lua_ShakeCamera);
    lua_register(L, "FadeIn", Lua_FadeIn);
    lua_register(L, "FadeOut", Lua_FadeOut);
    lua_register(L, "HttpGet", Lua_HttpGet);
    lua_register(L, "HttpPost", Lua_HttpPost);
    lua_register(L, "TriggerEvent", Lua_TriggerEvent);
}

// ---- 基础系统 ----
int LuaAPI::Lua_Log(lua_State* L) {
    const char* msg = lua_tostring(L, 1);
    if (msg) std::cout << "[Lua] " << msg << std::endl;
    return 0;
}

int LuaAPI::Lua_Sleep(lua_State* L) {
    int ms = lua_tointeger(L, 1);
    if (ms > 0) std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    return 0;
}

int LuaAPI::Lua_Random(lua_State* L) {
    lua_pushinteger(L, rand());
    return 1;
}

int LuaAPI::Lua_RandomRange(lua_State* L) {
    int min = lua_tointeger(L, 1);
    int max = lua_tointeger(L, 2);
    if (min > max) { int tmp = min; min = max; max = tmp; }
    lua_pushinteger(L, min + rand() % (max - min + 1));
    return 1;
}

// ---- 场景管理 ----
int LuaAPI::Lua_LoadScene(lua_State* L) {
    const char* scene = lua_tostring(L, 1);
    if (scene) GameState::Instance().GetPlayer().sceneId = scene;
    return 0;
}

int LuaAPI::Lua_GetCurrentScene(lua_State* L) {
    lua_pushstring(L, GameState::Instance().GetPlayer().sceneId.c_str());
    return 1;
}

// ---- 存档系统 ----
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

int LuaAPI::Lua_GetSaveSlots(lua_State* L) {
    lua_newtable(L);
    for (int i = 1; i <= 5; ++i) {
        lua_pushinteger(L, i);
        std::string path = "save_" + std::to_string(i) + ".sav";
        FILE* f = fopen(path.c_str(), "rb");
        lua_pushboolean(L, f != nullptr);
        if (f) fclose(f);
        lua_settable(L, -3);
    }
    return 1;
}

// ---- 对话系统 ----
int LuaAPI::Lua_StartDialogue(lua_State* L) {
    int n = lua_gettop(L);
    if (n < 2 || !g_DialogueMgr) return 0;
    std::vector<RPG::DialogueLine> lines;
    for (int i = 1; i <= n; i += 2) {
        if (i + 1 > n) break;
        const char* speaker = lua_tostring(L, i);
        const char* text    = lua_tostring(L, i + 1);
        if (speaker && text) lines.push_back({speaker, text});
    }
    if (!lines.empty()) g_DialogueMgr->StartDialogue(lines);
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

// ---- 物品系统 ----
int LuaAPI::Lua_GiveItem(lua_State* L) {
    const char* id = lua_tostring(L, 1);
    int count = lua_tointeger(L, 2);
    if (count <= 0) count = 1;
    if (id) GameState::Instance().GetPlayer().inventory[id] += count;
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
    lua_pushinteger(L, (it != inv.end()) ? it->second : 0);
    return 1;
}

int LuaAPI::Lua_HasItem(lua_State* L) {
    const char* id = lua_tostring(L, 1);
    int need = lua_tointeger(L, 2);
    if (need <= 0) need = 1;
    auto& inv = GameState::Instance().GetPlayer().inventory;
    auto it = inv.find(id);
    lua_pushboolean(L, (it != inv.end() && it->second >= need));
    return 1;
}

int LuaAPI::Lua_UseItem(lua_State* L) {
    const char* id = lua_tostring(L, 1);
    if (!id) return 0;
    const RPG::Item* proto = RPG::Inventory::GetItemPrototype(id);
    if (proto && proto->onUse) {
        std::cout << "[Lua] 使用物品: " << id << std::endl;
    }
    auto& inv = GameState::Instance().GetPlayer().inventory;
    auto it = inv.find(id);
    if (it != inv.end()) {
        it->second--;
        if (it->second <= 0) inv.erase(it);
    }
    return 0;
}

// ---- 任务系统 ----
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
    const char* questId = lua_tostring(L, 1);
    int objIndex = lua_tointeger(L, 2);
    int progress = lua_tointeger(L, 3);
    if (questId && g_QuestMgr)
        g_QuestMgr->UpdateObjective(questId, objIndex, progress);
    return 0;
}

// ---- 角色属性 ----
int LuaAPI::Lua_GetHP(lua_State* L) {
    lua_pushinteger(L, GameState::Instance().GetPlayer().hp);
    return 1;
}

int LuaAPI::Lua_SetHP(lua_State* L) {
    int val = lua_tointeger(L, 1);
    GameState::Instance().GetPlayer().hp = val;
    return 0;
}

int LuaAPI::Lua_GetMP(lua_State* L) {
    lua_pushinteger(L, GameState::Instance().GetPlayer().mp);
    return 1;
}

int LuaAPI::Lua_SetMP(lua_State* L) {
    int val = lua_tointeger(L, 1);
    GameState::Instance().GetPlayer().mp = val;
    return 0;
}

int LuaAPI::Lua_GetLevel(lua_State* L) {
    lua_pushinteger(L, GameState::Instance().GetPlayer().level);
    return 1;
}

int LuaAPI::Lua_AddExp(lua_State* L) {
    int exp = lua_tointeger(L, 1);
    auto& p = GameState::Instance().GetPlayer();
    p.exp += exp;
    while (p.exp >= p.level * 100) {
        p.exp -= p.level * 100;
        p.level++;
        p.maxHp += 10;
        p.hp = p.maxHp;
        p.maxMp += 5;
        p.mp = p.maxMp;
        std::cout << "[Lua] 升级！现在等级 " << p.level << std::endl;
    }
    lua_pushinteger(L, p.level);
    return 1;
}

int LuaAPI::Lua_Heal(lua_State* L) {
    auto& p = GameState::Instance().GetPlayer();
    p.hp = p.maxHp;
    p.mp = p.maxMp;
    return 0;
}

int LuaAPI::Lua_Damage(lua_State* L) {
    int dmg = lua_tointeger(L, 1);
    auto& p = GameState::Instance().GetPlayer();
    p.hp -= dmg;
    if (p.hp < 0) p.hp = 0;
    return 0;
}

// ---- 战斗系统 ----
int LuaAPI::Lua_StartBattle(lua_State* L) {
    if (!g_BattleSystem) {
        g_BattleSystem = RPG::BattleSystem::Create();
    }
    auto enemy = std::make_shared<RPG::BattleEntity>();
    enemy->name = "史莱姆";
    enemy->stats.hp = 30;
    enemy->stats.maxHp = 30;
    enemy->stats.attack = 8;
    enemy->stats.defense = 2;
    enemy->stats.speed = 5;
    enemy->stats.expReward = 10;
    std::vector<std::shared_ptr<RPG::BattleEntity>> enemies = {enemy};
    std::vector<std::shared_ptr<RPG::BattleEntity>> allies;
    g_BattleSystem->StartBattle(allies, enemies);
    lua_pushboolean(L, true);
    return 1;
}

int LuaAPI::Lua_IsBattleOver(lua_State* L) {
    lua_pushboolean(L, g_BattleSystem ? g_BattleSystem->IsBattleOver() : true);
    return 1;
}

int LuaAPI::Lua_ExecuteBattleCommand(lua_State* L) {
    int actorIdx = lua_tointeger(L, 1);
    const char* skillName = lua_tostring(L, 2);
    int targetIdx = lua_tointeger(L, 3);
    if (!g_BattleSystem || g_BattleSystem->GetState() != RPG::BattleState::PlayerTurn) return 0;

    std::shared_ptr<RPG::BattleSkill> skill;
    if (skillName && strlen(skillName) > 0) {
        skill = std::make_shared<RPG::BattleSkill>();
        skill->name = skillName;
        skill->power = 15;
        skill->mpCost = 5;
        skill->targetType = RPG::BattleSkill::SingleEnemy;
        skill->effect = [skillName](RPG::BattleEntity& user, RPG::BattleEntity& target) {
            int dmg = std::max(1, user.stats.attack + 5 - target.stats.defense);
            target.takeDamage(dmg);
            std::cout << "[Lua Battle] " << user.name << " 使用 " << skillName << " 造成 " << dmg << " 点伤害\n";
        };
    }
    g_BattleSystem->SubmitPlayerAction(actorIdx, skill, targetIdx);
    g_BattleSystem->Update(0.0f);
    return 0;
}

// ---- 音频系统 ----
int LuaAPI::Lua_PlaySound(lua_State* L) {
    const char* id = lua_tostring(L, 1);
    if (id) AudioManager::Instance().PlaySound(id);
    return 0;
}

int LuaAPI::Lua_PlayMusic(lua_State* L) {
    const char* id = lua_tostring(L, 1);
    if (id) AudioManager::Instance().PlayMusic(id);
    return 0;
}

int LuaAPI::Lua_StopMusic(lua_State* L) {
    AudioManager::Instance().StopMusic();
    return 0;
}

int LuaAPI::Lua_SetMusicVolume(lua_State* L) {
    float vol = lua_tonumber(L, 1);
    AudioManager::Instance().SetMusicVolume(vol);
    return 0;
}

// ---- 全局标记 ----
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
    lua_pushstring(L, (it != flags.end()) ? it->second.c_str() : "");
    return 1;
}

int LuaAPI::Lua_HasFlag(lua_State* L) {
    const char* key = lua_tostring(L, 1);
    lua_pushboolean(L, GameState::Instance().GetPlayer().flags.find(key) != GameState::Instance().GetPlayer().flags.end());
    return 1;
}

int LuaAPI::Lua_ClearFlag(lua_State* L) {
    const char* key = lua_tostring(L, 1);
    GameState::Instance().GetPlayer().flags.erase(key);
    return 0;
}

// ---- 摄像机 ----
int LuaAPI::Lua_SetCameraPos(lua_State* L) { return 0; }
int LuaAPI::Lua_GetCameraPos(lua_State* L) { lua_pushnumber(L, 0); lua_pushnumber(L, 0); return 2; }
int LuaAPI::Lua_ShakeCamera(lua_State* L) { return 0; }

// ---- 过渡效果 ----
int LuaAPI::Lua_FadeIn(lua_State* L) { return 0; }
int LuaAPI::Lua_FadeOut(lua_State* L) { return 0; }

// ---- HTTP 请求 ----
int LuaAPI::Lua_HttpGet(lua_State* L) { lua_pushstring(L, ""); return 1; }
int LuaAPI::Lua_HttpPost(lua_State* L) { lua_pushstring(L, ""); return 1; }

// ---- 事件系统 ----
int LuaAPI::Lua_TriggerEvent(lua_State* L) {
    const char* name = lua_tostring(L, 1);
    if (name && g_EventMgr) g_EventMgr->TriggerEvent(name);
    return 0;
}

} // namespace MoLin
