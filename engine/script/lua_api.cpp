#include "lua_api.h"
#include "../game/game_state.h"
#include "../rpg/inventory.h"
#include "../rpg/quest.h"
#include "../rpg/battle.h"
#include "../rpg/dialogue.h"
#include "../rpg/event_system.h"
#include "../audio/audio_player.h"
#include "../resource/resource_manager.h"
#include <iostream>
#include <memory>

namespace MoLin {

static std::shared_ptr<RPG::DialogueManager> g_DialogueMgr;
static std::shared_ptr<RPG::QuestManager> g_QuestMgr;
static std::shared_ptr<RPG::BattleSystem> g_BattleSystem;
static std::shared_ptr<RPG::EventManager> g_EventMgr;
static SDL_Renderer* g_Renderer = nullptr;

void SetDialogueManager(std::shared_ptr<RPG::DialogueManager> mgr) { g_DialogueMgr = mgr; }
void SetQuestManager(std::shared_ptr<RPG::QuestManager> mgr) { g_QuestMgr = mgr; }
void SetBattleSystem(std::shared_ptr<RPG::BattleSystem> sys) { g_BattleSystem = sys; }
void SetEventManager(std::shared_ptr<RPG::EventManager> mgr) { g_EventMgr = mgr; }
void SetRenderer(SDL_Renderer* r) { g_Renderer = r; }

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

int LuaAPI::Lua_Log(lua_State* L) { const char* m=lua_tostring(L,1); if(m) std::cout<<"[Lua] "<<m<<std::endl; return 0; }
int LuaAPI::Lua_LoadScene(lua_State* L) { const char* s=lua_tostring(L,1); if(s) GameState::Instance().GetPlayer().sceneId=s; return 0; }
int LuaAPI::Lua_GetCurrentScene(lua_State* L) { lua_pushstring(L, GameState::Instance().GetPlayer().sceneId.c_str()); return 1; }

int LuaAPI::Lua_StartDialogue(lua_State* L) {
    int n=lua_gettop(L); if(n<2)return 0;
    std::vector<RPG::DialogueLine> lines;
    for(int i=1;i<=n;i+=2){
        if(i+1>n)break;
        const char* s=lua_tostring(L,i), *t=lua_tostring(L,i+1);
        if(s&&t)lines.push_back({s,t});
    }
    if(g_DialogueMgr&&!lines.empty())g_DialogueMgr->StartDialogue(lines);
    return 0;
}
int LuaAPI::Lua_ShowNextLine(lua_State* L) { if(g_DialogueMgr)g_DialogueMgr->ShowNextLine(); return 0; }
int LuaAPI::Lua_IsDialogueActive(lua_State* L) { lua_pushboolean(L,g_DialogueMgr?g_DialogueMgr->IsActive():false); return 1; }

int LuaAPI::Lua_SaveGame(lua_State* L) { int s=lua_tointeger(L,1); if(s<=0)s=1; lua_pushboolean(L,GameState::Instance().Save(s)); return 1; }
int LuaAPI::Lua_LoadGame(lua_State* L) { int s=lua_tointeger(L,1); if(s<=0)s=1; lua_pushboolean(L,GameState::Instance().Load(s)); return 1; }
int LuaAPI::Lua_DeleteSave(lua_State* L) { int s=lua_tointeger(L,1); if(s<=0)s=1; lua_pushboolean(L,GameState::Instance().DeleteSave(s)); return 1; }

int LuaAPI::Lua_GiveItem(lua_State* L) { const char* id=lua_tostring(L,1); int c=lua_tointeger(L,2); if(c<=0)c=1; if(id)GameState::Instance().GetPlayer().inventory[id]+=c; return 0; }
int LuaAPI::Lua_RemoveItem(lua_State* L) {
    const char* id=lua_tostring(L,1); int c=lua_tointeger(L,2); if(c<=0)c=1;
    auto& inv=GameState::Instance().GetPlayer().inventory; auto it=inv.find(id);
    if(it!=inv.end()){ it->second-=c; if(it->second<=0)inv.erase(it); lua_pushboolean(L,true); }else lua_pushboolean(L,false);
    return 1;
}
int LuaAPI::Lua_GetItemCount(lua_State* L) { auto& inv=GameState::Instance().GetPlayer().inventory; auto it=inv.find(lua_tostring(L,1)); lua_pushinteger(L,it!=inv.end()?it->second:0); return 1; }
int LuaAPI::Lua_HasItem(lua_State* L) { auto& inv=GameState::Instance().GetPlayer().inventory; auto it=inv.find(lua_tostring(L,1)); int nd=lua_tointeger(L,2); if(nd<=0)nd=1; lua_pushboolean(L,it!=inv.end()&&it->second>=nd); return 1; }

int LuaAPI::Lua_StartQuest(lua_State* L) { const char* id=lua_tostring(L,1); if(id&&g_QuestMgr){g_QuestMgr->StartQuest(id); GameState::Instance().GetPlayer().quests[id]=1;} return 0; }
int LuaAPI::Lua_CompleteQuest(lua_State* L) { const char* id=lua_tostring(L,1); if(id)GameState::Instance().GetPlayer().quests[id]=2; return 0; }
int LuaAPI::Lua_FailQuest(lua_State* L) { const char* id=lua_tostring(L,1); if(id)GameState::Instance().GetPlayer().quests[id]=-1; return 0; }
int LuaAPI::Lua_UpdateObjective(lua_State* L) { const char* id=lua_tostring(L,1); int idx=lua_tointeger(L,2),prog=lua_tointeger(L,3); if(id&&g_QuestMgr)g_QuestMgr->UpdateObjective(id,idx,prog); return 0; }

int LuaAPI::Lua_GetPlayerHP(lua_State* L) { lua_pushinteger(L,GameState::Instance().GetPlayer().hp); return 1; }
int LuaAPI::Lua_SetPlayerHP(lua_State* L) { GameState::Instance().GetPlayer().hp=lua_tointeger(L,1); return 0; }
int LuaAPI::Lua_GetPlayerMP(lua_State* L) { lua_pushinteger(L,GameState::Instance().GetPlayer().mp); return 1; }
int LuaAPI::Lua_SetPlayerMP(lua_State* L) { GameState::Instance().GetPlayer().mp=lua_tointeger(L,1); return 0; }
int LuaAPI::Lua_GetPlayerLevel(lua_State* L) { lua_pushinteger(L,GameState::Instance().GetPlayer().level); return 1; }
int LuaAPI::Lua_AddExp(lua_State* L) { int e=lua_tointeger(L,1); auto& p=GameState::Instance().GetPlayer(); p.exp+=e; if(p.exp>=p.level*100){p.exp-=p.level*100;p.level++;} lua_pushinteger(L,p.level); return 1; }

int LuaAPI::Lua_StartBattle(lua_State* L) {
    if(g_BattleSystem){ std::vector<std::shared_ptr<RPG::BattleEntity>> al,en; g_BattleSystem->StartBattle(al,en); lua_pushboolean(L,true); }else lua_pushboolean(L,false);
    return 1;
}
int LuaAPI::Lua_IsBattleOver(lua_State* L) { lua_pushboolean(L,g_BattleSystem?g_BattleSystem->IsBattleOver():true); return 1; }
int LuaAPI::Lua_TriggerEvent(lua_State* L) { const char* n=lua_tostring(L,1); if(n&&g_EventMgr)g_EventMgr->TriggerEvent(n); return 0; }

int LuaAPI::Lua_PlaySound(lua_State* L) { const char* p=lua_tostring(L,1); if(p&&g_Renderer){auto* c=ResourceManager::Instance().LoadSound(p);AudioPlayer::Instance().PlaySound(c);} return 0; }
int LuaAPI::Lua_PlayMusic(lua_State* L) { const char* p=lua_tostring(L,1); if(p){auto* m=ResourceManager::Instance().LoadMusic(p);AudioPlayer::Instance().PlayMusic(m);} return 0; }
int LuaAPI::Lua_StopMusic(lua_State* L) { AudioPlayer::Instance().StopMusic(); return 0; }

int LuaAPI::Lua_SetFlag(lua_State* L) { const char* k=lua_tostring(L,1),*v=lua_tostring(L,2); if(k&&v)GameState::Instance().GetPlayer().flags[k]=v; return 0; }
int LuaAPI::Lua_GetFlag(lua_State* L) { auto& fl=GameState::Instance().GetPlayer().flags; auto it=fl.find(lua_tostring(L,1)); lua_pushstring(L,it!=fl.end()?it->second.c_str():""); return 1; }
int LuaAPI::Lua_HasFlag(lua_State* L) { lua_pushboolean(L,GameState::Instance().GetPlayer().flags.find(lua_tostring(L,1))!=GameState::Instance().GetPlayer().flags.end()); return 1; }

} // namespace MoLin
