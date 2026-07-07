#include "simple_crypto.h"
#include <cstring>
#include <algorithm>

namespace MoLin::Crypto {

void SimpleCrypto::EncryptDecrypt(std::vector<uint8_t>& data, const std::string& key) {
    if (key.empty()) return;
    size_t keyLen = key.size();
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] ^= static_cast<uint8_t>(key[i % keyLen]) ^ 0xAA; // 加固定掩码增强混淆
    }
}

uint32_t SimpleCrypto::ComputeChecksum(const std::vector<uint8_t>& data, uint32_t seed) {
    uint32_t sum = seed;
    for (auto byte : data) {
        sum += byte;
        sum = (sum << 5) | (sum >> 27); // 循环左移5位
        sum ^= 0xEDB88320;              // 标准 CRC32 多项式简化版
    }
    return sum;
}

bool SimpleCrypto::ValidateChecksum(const std::vector<uint8_t>& data, uint32_t checksum, uint32_t seed) {
    return ComputeChecksum(data, seed) == checksum;
}

static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string SimpleCrypto::Base64Encode(const std::vector<uint8_t>& data) {
    std::string result;
    int val = 0, valb = -6;
    for (uint8_t c : data) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            result.push_back(base64_table[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) result.push_back(base64_table[((val << 8) >> (valb + 8)) & 0x3F]);
    while (result.size() % 4) result.push_back('=');
    return result;
}

std::vector<uint8_t> SimpleCrypto::Base64Decode(const std::string& str) {
    std::vector<uint8_t> result;
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[base64_table[i]] = i;
    int val = 0, valb = -8;
    for (char c : str) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            result.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return result;
}

} // namespace MoLin::Crypto
