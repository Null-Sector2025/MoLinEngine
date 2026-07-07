#include "game_state.h"
#include <cstring>
#include <iostream>

namespace MoLin {

std::string GameState::GetSlotPath(int slot) const {
    return "save_" + std::to_string(slot) + ".sav";
}

bool GameState::Save(int slot) {
    std::vector<uint8_t> buffer;
    Serialize(buffer);
    return m_SaveManager.SaveToFile(GetSlotPath(slot), buffer);
}

bool GameState::Load(int slot) {
    std::vector<uint8_t> buffer;
    if (!m_SaveManager.LoadFromFile(GetSlotPath(slot), buffer))
        return false;
    return Deserialize(buffer);
}

bool GameState::DeleteSave(int slot) {
    return std::remove(GetSlotPath(slot).c_str()) == 0;
}

// ---- 序列化实现 ----
void GameState::WriteInt(std::vector<uint8_t>& buf, int32_t val) {
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&val),
               reinterpret_cast<const uint8_t*>(&val) + sizeof(val));
}
void GameState::WriteFloat(std::vector<uint8_t>& buf, float val) {
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&val),
               reinterpret_cast<const uint8_t*>(&val) + sizeof(val));
}
void GameState::WriteString(std::vector<uint8_t>& buf, const std::string& s) {
    WriteInt(buf, static_cast<int32_t>(s.size()));
    buf.insert(buf.end(), s.begin(), s.end());
}
int32_t GameState::ReadInt(const std::vector<uint8_t>& buf, size_t& offset) {
    if (offset + sizeof(int32_t) > buf.size()) return 0;
    int32_t val;
    std::memcpy(&val, buf.data() + offset, sizeof(val));
    offset += sizeof(val);
    return val;
}
float GameState::ReadFloat(const std::vector<uint8_t>& buf, size_t& offset) {
    if (offset + sizeof(float) > buf.size()) return 0.0f;
    float val;
    std::memcpy(&val, buf.data() + offset, sizeof(val));
    offset += sizeof(val);
    return val;
}
std::string GameState::ReadString(const std::vector<uint8_t>& buf, size_t& offset) {
    int32_t len = ReadInt(buf, offset);
    if (len < 0 || offset + len > buf.size()) return "";
    std::string s(buf.begin() + offset, buf.begin() + offset + len);
    offset += len;
    return s;
}

void GameState::Serialize(std::vector<uint8_t>& buffer) const {
    WriteString(buffer, m_Player.sceneId);
    WriteFloat(buffer, m_Player.posX);
    WriteFloat(buffer, m_Player.posY);
    WriteInt(buffer, m_Player.level);
    WriteInt(buffer, m_Player.hp);
    WriteInt(buffer, m_Player.maxHp);
    WriteInt(buffer, m_Player.mp);
    WriteInt(buffer, m_Player.maxMp);
    WriteInt(buffer, m_Player.exp);

    WriteInt(buffer, static_cast<int32_t>(m_Player.inventory.size()));
    for (auto& [id, cnt] : m_Player.inventory) {
        WriteString(buffer, id);
        WriteInt(buffer, cnt);
    }
    WriteInt(buffer, static_cast<int32_t>(m_Player.quests.size()));
    for (auto& [id, status] : m_Player.quests) {
        WriteString(buffer, id);
        WriteInt(buffer, status);
    }
    WriteInt(buffer, static_cast<int32_t>(m_Player.flags.size()));
    for (auto& [key, val] : m_Player.flags) {
        WriteString(buffer, key);
        WriteString(buffer, val);
    }
}

bool GameState::Deserialize(const std::vector<uint8_t>& buffer) {
    size_t offset = 0;
    m_Player.sceneId = ReadString(buffer, offset);
    m_Player.posX = ReadFloat(buffer, offset);
    m_Player.posY = ReadFloat(buffer, offset);
    m_Player.level = ReadInt(buffer, offset);
    m_Player.hp = ReadInt(buffer, offset);
    m_Player.maxHp = ReadInt(buffer, offset);
    m_Player.mp = ReadInt(buffer, offset);
    m_Player.maxMp = ReadInt(buffer, offset);
    m_Player.exp = ReadInt(buffer, offset);

    int invCount = ReadInt(buffer, offset);
    m_Player.inventory.clear();
    for (int i = 0; i < invCount; ++i) {
        std::string id = ReadString(buffer, offset);
        int cnt = ReadInt(buffer, offset);
        m_Player.inventory[id] = cnt;
    }
    int questCount = ReadInt(buffer, offset);
    m_Player.quests.clear();
    for (int i = 0; i < questCount; ++i) {
        std::string id = ReadString(buffer, offset);
        int status = ReadInt(buffer, offset);
        m_Player.quests[id] = status;
    }
    int flagCount = ReadInt(buffer, offset);
    m_Player.flags.clear();
    for (int i = 0; i < flagCount; ++i) {
        std::string key = ReadString(buffer, offset);
        std::string val = ReadString(buffer, offset);
        m_Player.flags[key] = val;
    }
    return offset <= buffer.size();
}

} // namespace MoLin
