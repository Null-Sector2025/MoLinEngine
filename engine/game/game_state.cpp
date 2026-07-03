#include "game_state.h"
#include <cstring>

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
    if (!m_SaveManager.LoadFromFile(GetSlotPath(slot), buffer)) {
        return false;
    }
    return Deserialize(buffer);
}

void GameState::Serialize(std::vector<uint8_t>& buffer) const {
    // 简单二进制序列化：依次写入各字段
    auto writeInt = [&](int32_t val) {
        buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&val), reinterpret_cast<const uint8_t*>(&val) + sizeof(val));
    };
    auto writeFloat = [&](float val) {
        buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&val), reinterpret_cast<const uint8_t*>(&val) + sizeof(val));
    };
    auto writeString = [&](const std::string& s) {
        int32_t len = s.size();
        writeInt(len);
        buffer.insert(buffer.end(), s.begin(), s.end());
    };

    writeString(m_Player.sceneId);
    writeFloat(m_Player.posX);
    writeFloat(m_Player.posY);
    writeInt(m_Player.level);
    writeInt(m_Player.hp); writeInt(m_Player.maxHp);
    writeInt(m_Player.mp); writeInt(m_Player.maxMp);

    writeInt(m_Player.inventory.size());
    for (auto& [id, count] : m_Player.inventory) {
        writeString(id);
        writeInt(count);
    }
    writeInt(m_Player.quests.size());
    for (auto& [id, status] : m_Player.quests) {
        writeString(id);
        writeInt(status);
    }
}

bool GameState::Deserialize(const std::vector<uint8_t>& buffer) {
    size_t offset = 0;
    auto readInt = [&]() -> int32_t {
        if (offset + sizeof(int32_t) > buffer.size()) return 0;
        int32_t val;
        std::memcpy(&val, buffer.data() + offset, sizeof(val));
        offset += sizeof(val);
        return val;
    };
    auto readFloat = [&]() -> float {
        if (offset + sizeof(float) > buffer.size()) return 0.0f;
        float val;
        std::memcpy(&val, buffer.data() + offset, sizeof(val));
        offset += sizeof(val);
        return val;
    };
    auto readString = [&]() -> std::string {
        int32_t len = readInt();
        if (len < 0 || offset + len > buffer.size()) return "";
        std::string s(buffer.begin() + offset, buffer.begin() + offset + len);
        offset += len;
        return s;
    };

    m_Player.sceneId = readString();
    m_Player.posX = readFloat();
    m_Player.posY = readFloat();
    m_Player.level = readInt();
    m_Player.hp = readInt(); m_Player.maxHp = readInt();
    m_Player.mp = readInt(); m_Player.maxMp = readInt();

    int invCount = readInt();
    m_Player.inventory.clear();
    for (int i = 0; i < invCount; ++i) {
        std::string id = readString();
        int count = readInt();
        m_Player.inventory[id] = count;
    }
    int questCount = readInt();
    m_Player.quests.clear();
    for (int i = 0; i < questCount; ++i) {
        std::string id = readString();
        int status = readInt();
        m_Player.quests[id] = status;
    }
    return true;
}

} // namespace MoLin
