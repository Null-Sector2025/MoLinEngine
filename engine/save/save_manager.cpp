#include "save_manager.h"
#include <iostream>
#include <cstring>

namespace MoLin {

SaveManager::SaveManager(const std::string& cryptoKey) : m_CryptoKey(cryptoKey) {}

bool SaveManager::SaveToFile(const std::string& path, const std::vector<uint8_t>& data) {
    std::vector<uint8_t> encrypted = data;
    Crypto::SimpleCrypto::EncryptDecrypt(encrypted, m_CryptoKey);
    uint32_t checksum = Crypto::SimpleCrypto::ComputeChecksum(encrypted);

    std::ofstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "[Save] Failed to open " << path << " for writing" << std::endl;
        return false;
    }

    SaveHeader header;
    header.dataSize = static_cast<uint32_t>(encrypted.size());
    header.checksum = checksum;
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    file.write(reinterpret_cast<const char*>(encrypted.data()), encrypted.size());
    file.close();
    std::cout << "[Save] Saved to " << path << " (" << encrypted.size() << " bytes)" << std::endl;
    return true;
}

bool SaveManager::LoadFromFile(const std::string& path, std::vector<uint8_t>& outData) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "[Save] Failed to open " << path << std::endl;
        return false;
    }

    SaveHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (!file || std::memcmp(header.magic, "MLS1", 4) != 0) {
        std::cerr << "[Save] Invalid save file format" << std::endl;
        return false;
    }

    std::vector<uint8_t> encrypted(header.dataSize);
    file.read(reinterpret_cast<char*>(encrypted.data()), header.dataSize);
    if (!file) {
        std::cerr << "[Save] Failed to read data" << std::endl;
        return false;
    }

    // 校验
    if (!Crypto::SimpleCrypto::ValidateChecksum(encrypted, header.checksum)) {
        std::cerr << "[Save] Checksum mismatch, file may be corrupted" << std::endl;
        return false;
    }

    outData = encrypted;
    Crypto::SimpleCrypto::EncryptDecrypt(outData, m_CryptoKey); // 解密
    std::cout << "[Save] Loaded from " << path << " successfully" << std::endl;
    return true;
}

} // namespace MoLin
