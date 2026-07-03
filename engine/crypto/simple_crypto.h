#pragma once
#include <vector>
#include <string>

namespace MoLin::Crypto {

// 自创加密算法：基于密钥的循环异或 + 校验和
class SimpleCrypto {
public:
    static void EncryptDecrypt(std::vector<uint8_t>& data, const std::string& key);
    static uint32_t ComputeChecksum(const std::vector<uint8_t>& data);
    static bool ValidateChecksum(const std::vector<uint8_t>& data, uint32_t checksum);
};

} // namespace MoLin::Crypto
