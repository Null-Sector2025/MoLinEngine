#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include "crypto/simple_crypto.h"

namespace MoLin {

struct SaveHeader {
    char magic[4] = {'M','L','S','1'};
    uint32_t dataSize = 0;
    uint32_t checksum = 0;
    uint32_t timestamp = 0;
};

struct SaveSlotInfo {
    int slot;
    uint32_t timestamp;
    std::string sceneName;
    int playerLevel;
};

class SaveManager {
public:
    SaveManager(const std::string& cryptoKey = "MoLinEngineKey");

    bool SaveToFile(const std::string& path, const std::vector<uint8_t>& data);
    bool LoadFromFile(const std::string& path, std::vector<uint8_t>& outData);
    void SetCryptoKey(const std::string& key) { m_CryptoKey = key; }
    static std::vector<SaveSlotInfo> GetSaveSlots();

private:
    std::string m_CryptoKey;
};

} // namespace MoLin
