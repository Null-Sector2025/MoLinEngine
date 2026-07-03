#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include "../crypto/simple_crypto.h"

namespace MoLin {

class SaveManager {
public:
    SaveManager(const std::string& key = "MoLinEngineKey");

    bool SaveToFile(const std::string& path, const std::vector<uint8_t>& data);
    bool LoadFromFile(const std::string& path, std::vector<uint8_t>& outData);

private:
    std::string m_CryptoKey;
    void WriteHeader(std::ofstream& file, const std::vector<uint8_t>& data);
    bool ReadHeader(std::ifstream& file, std::vector<uint8_t>& outData);
};

} // namespace MoLin
