#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include "../save/save_manager.h"

namespace MoLin {

struct PlayerState {
    std::string sceneId;
    float posX, posY;
    int level = 1;
    int hp = 100, maxHp = 100;
    int mp = 50, maxMp = 50;
    // 物品 ID -> 数量
    std::unordered_map<std::string, int> inventory;
    // 任务 ID -> 状态 (0=未激活,1=进行中,2=完成)
    std::unordered_map<std::string, int> quests;
};

class GameState {
public:
    static GameState& Instance() {
        static GameState state;
        return state;
    }

    PlayerState& GetPlayer() { return m_Player; }
    void SetPlayer(const PlayerState& ps) { m_Player = ps; }

    bool Save(int slot = 1);
    bool Load(int slot = 1);

private:
    GameState() = default;
    PlayerState m_Player;
    SaveManager m_SaveManager;

    std::string GetSlotPath(int slot) const;
    void Serialize(std::vector<uint8_t>& buffer) const;
    bool Deserialize(const std::vector<uint8_t>& buffer);
};

} // namespace MoLin
