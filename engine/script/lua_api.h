#pragma once
#include "lua_bind.h"
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <vector>

// 前置声明
namespace MoLin::RPG {
    class DialogueManager;
    class QuestManager;
    class BattleSystem;
    class EventManager;
    struct DialogueLine;
}

namespace MoLin {

class LuaAPI {
public:
    static void RegisterAll(LuaBind& lua);

    static void SetDialogueManager(std::shared_ptr<RPG::DialogueManager> mgr);
    static void SetQuestManager(std::shared_ptr<RPG::QuestManager> mgr);
    static void SetBattleSystem(std::shared_ptr<RPG::BattleSystem> sys);
    static void SetEventManager(std::shared_ptr<RPG::EventManager> mgr);
    static void SetRenderer(SDL_Renderer* r);

private:
    // 系统
    static int Lua_Log(lua_State* L);
    static int Lua_Sleep(lua_State* L);
    static int Lua_Random(lua_State* L);
    static int Lua_RandomRange(lua_State* L);

    // 场景
    static int Lua_LoadScene(lua_State* L);
    static int Lua_GetCurrentScene(lua_State* L);

    // 存档
    static int Lua_SaveGame(lua_State* L);
    static int Lua_LoadGame(lua_State* L);
    static int Lua_DeleteSave(lua_State* L);
    static int Lua_GetSaveSlots(lua_State* L);

    // 对话
    static int Lua_StartDialogue(lua_State* L);
    static int Lua_ShowNextLine(lua_State* L);
    static int Lua_IsDialogueActive(lua_State* L);

    // 物品
    static int Lua_GiveItem(lua_State* L);
    static int Lua_RemoveItem(lua_State* L);
    static int Lua_GetItemCount(lua_State* L);
    static int Lua_HasItem(lua_State* L);
    static int Lua_UseItem(lua_State* L);

    // 任务
    static int Lua_StartQuest(lua_State* L);
    static int Lua_CompleteQuest(lua_State* L);
    static int Lua_FailQuest(lua_State* L);
    static int Lua_UpdateObjective(lua_State* L);

    // 角色（统一命名）
    static int Lua_GetHP(lua_State* L);
    static int Lua_SetHP(lua_State* L);
    static int Lua_GetMP(lua_State* L);
    static int Lua_SetMP(lua_State* L);
    static int Lua_GetLevel(lua_State* L);
    static int Lua_AddExp(lua_State* L);
    static int Lua_Heal(lua_State* L);
    static int Lua_Damage(lua_State* L);

    // 战斗
    static int Lua_StartBattle(lua_State* L);
    static int Lua_IsBattleOver(lua_State* L);
    static int Lua_ExecuteBattleCommand(lua_State* L);

    // 音频
    static int Lua_PlaySound(lua_State* L);
    static int Lua_PlayMusic(lua_State* L);
    static int Lua_StopMusic(lua_State* L);
    static int Lua_SetMusicVolume(lua_State* L);

    // 全局标记
    static int Lua_SetFlag(lua_State* L);
    static int Lua_GetFlag(lua_State* L);
    static int Lua_HasFlag(lua_State* L);
    static int Lua_ClearFlag(lua_State* L);

    // 摄像机
    static int Lua_SetCameraPos(lua_State* L);
    static int Lua_GetCameraPos(lua_State* L);
    static int Lua_ShakeCamera(lua_State* L);

    // 过渡
    static int Lua_FadeIn(lua_State* L);
    static int Lua_FadeOut(lua_State* L);

    // HTTP
    static int Lua_HttpGet(lua_State* L);
    static int Lua_HttpPost(lua_State* L);

    // 事件
    static int Lua_TriggerEvent(lua_State* L);
};

} // namespace MoLin
