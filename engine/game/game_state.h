#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include "save/save_manager.h"

namespace MoLin {

struct PlayerState {
    std::string sceneId = "default";
    float posX = 0.0f, posY = 0.0f;
    int level = 1;
    int hp = 100, maxHp = 100;
    int mp = 50, maxMp = 50;
    int exp = 0;
    std::unordered_map<std::string, int> inventory; // 物品ID → 数量
    std::unordered_map<std::string, int> quests;    // 任务ID → 状态(0未激活,1进行中,2完成)
    std::unordered_map<std::string, std::string> flags; // 全局标记
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
    bool DeleteSave(int slot);

private:
    GameState() = default;
    PlayerState m_Player;
    SaveManager m_SaveManager;

    std::string GetSlotPath(int slot) const;
    void Serialize(std::vector<uint8_t>& buffer) const;
    bool Deserialize(const std::vector<uint8_t>& buffer);

    // 序列化辅助函数
    static void WriteInt(std::vector<uint8_t>& buf, int32_t val);
    static void WriteFloat(std::vector<uint8_t>& buf, float val);
    static void WriteString(std::vector<uint8_t>& buf, const std::string& s);
    static int32_t ReadInt(const std::vector<uint8_t>& buf, size_t& offset);
    static float ReadFloat(const std::vector<uint8_t>& buf, size_t& offset);
    static std::string ReadString(const std::vector<uint8_t>& buf, size_t& offset);
};

} // namespace MoLin
