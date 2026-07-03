#include "simple_crypto.h"
#include <cstring>

namespace MoLin::Crypto {

void SimpleCrypto::EncryptDecrypt(std::vector<uint8_t>& data, const std::string& key) {
    size_t keyLen = key.size();
    if (keyLen == 0) return;
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] ^= key[i % keyLen];
    }
}

uint32_t SimpleCrypto::ComputeChecksum(const std::vector<uint8_t>& data) {
    uint32_t sum = 0;
    for (auto byte : data) {
        sum += byte;
        sum = (sum << 1) | (sum >> 31); // 简单循环
    }
    return sum;
}

bool SimpleCrypto::ValidateChecksum(const std::vector<uint8_t>& data, uint32_t checksum) {
    return ComputeChecksum(data) == checksum;
}

} // namespace MoLin::Crypto
