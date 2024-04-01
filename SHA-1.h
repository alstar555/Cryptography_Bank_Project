#ifndef CRYPTOGRAPHY_BANK_PROJECT_SHA_1_H
#define CRYPTOGRAPHY_BANK_PROJECT_SHA_1_H

#include <vector>

class SHA1 {
private:
    const uint32_t BLOCK_LEN = 64; // bytes

    std::vector<uint8_t> add_padding(const std::string& message);
    uint32_t left_rotate(uint32_t n, int shift, int b);

public:
    // Generate a 160 bit message digest
    std::string operator()(const std::string& message);
};

#endif //CRYPTOGRAPHY_BANK_PROJECT_SHA_1_H
