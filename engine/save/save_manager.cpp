#include "save_manager.h"
#include <iostream>

namespace MoLin {

SaveManager::SaveManager(const std::string& key) : m_CryptoKey(key) {}

bool SaveManager::SaveToFile(const std::string& path, const std::vector<uint8_t>& data) {
    std::vector<uint8_t> encrypted = data;
    Crypto::SimpleCrypto::EncryptDecrypt(encrypted, m_CryptoKey);
    uint32_t checksum = Crypto::SimpleCrypto::ComputeChecksum(encrypted);

    std::ofstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "[Save] Failed to open " << path << " for writing" << std::endl;
        return false;
    }
    WriteHeader(file, encrypted);
    // 写入校验和 (4 字节)
    file.write(reinterpret_cast<const char*>(&checksum), sizeof(checksum));
    file.close();
    std::cout << "[Save] Saved to " << path << std::endl;
    return true;
}

bool SaveManager::LoadFromFile(const std::string& path, std::vector<uint8_t>& outData) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "[Save] Failed to open " << path << " for reading" << std::endl;
        return false;
    }
    if (!ReadHeader(file, outData)) {
        return false;
    }
    // 读取校验和
    uint32_t checksum;
    file.read(reinterpret_cast<char*>(&checksum), sizeof(checksum));
    // 验证
    Crypto::SimpleCrypto::EncryptDecrypt(outData, m_CryptoKey); // 解密
    if (!Crypto::SimpleCrypto::ValidateChecksum(outData, checksum)) {
        std::cerr << "[Save] Checksum mismatch, file may be corrupted" << std::endl;
        return false;
    }
    std::cout << "[Save] Loaded from " << path << std::endl;
    return true;
}

void SaveManager::WriteHeader(std::ofstream& file, const std::vector<uint8_t>& data) {
    uint32_t size = data.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    file.write(reinterpret_cast<const char*>(data.data()), size);
}

bool SaveManager::ReadHeader(std::ifstream& file, std::vector<uint8_t>& outData) {
    uint32_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!file || size == 0) return false;
    outData.resize(size);
    file.read(reinterpret_cast<char*>(outData.data()), size);
    return file.good();
}

} // namespace MoLin
