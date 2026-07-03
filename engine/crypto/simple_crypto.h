#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace MoLin::Crypto {

class SimpleCrypto {
public:
    // 加密/解密（对称异或）
    static void EncryptDecrypt(std::vector<uint8_t>& data, const std::string& key);
    // 计算滚动校验和（带初始种子）
    static uint32_t ComputeChecksum(const std::vector<uint8_t>& data, uint32_t seed = 0);
    // 验证校验和
    static bool ValidateChecksum(const std::vector<uint8_t>& data, uint32_t checksum, uint32_t seed = 0);
    // Base64 编解码（用于存档可打印转换，可选）
    static std::string Base64Encode(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> Base64Decode(const std::string& str);
};

} // namespace MoLin::Crypto
