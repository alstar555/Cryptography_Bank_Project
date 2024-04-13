#ifndef CRYPTOGRAPHY_BANK_PROJECT_HMAC_H
#define CRYPTOGRAPHY_BANK_PROJECT_HMAC_H

#include <string>
#include "SHA-1.h"

class HMAC {
public:
    int block_size = 64;

    std::string operator()(const std::string& key, const std::string& message);

private:
    SHA1 sha1;

    std::string hash(const std::string& msg);
    std::string xor_string(const std::string& a, const std::string& b);
    std::string resize_key(const std::string& key);

};

#endif //CRYPTOGRAPHY_BANK_PROJECT_HMAC_H
