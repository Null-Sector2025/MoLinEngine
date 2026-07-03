#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "../crypto/simple_crypto.h"

namespace MoLin {

struct SaveHeader {
    char magic[4] = {'M','L','S','1'}; // 墨麟存档版本1
    uint32_t dataSize = 0;
    uint32_t checksum = 0;
};

class SaveManager {
public:
    SaveManager(const std::string& cryptoKey = "MoLinEngineKey");

    bool SaveToFile(const std::string& path, const std::vector<uint8_t>& data);
    bool LoadFromFile(const std::string& path, std::vector<uint8_t>& outData);
    void SetCryptoKey(const std::string& key) { m_CryptoKey = key; }

private:
    std::string m_CryptoKey;
};

} // namespace MoLin
