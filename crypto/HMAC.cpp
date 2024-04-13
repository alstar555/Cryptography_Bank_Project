#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "../Utils.h"
#include "SHA-1.h"
#include <boost/multiprecision/cpp_int.hpp>
#include "HMAC.h"

std::string HMAC::operator()(const std::string &key, const std::string &message) {
    std::string resized_key = resize_key(key);
    std::string outer_key_padded = xor_string(resized_key, std::string(block_size, '\x5c'));
    std::string inner_key_padded = xor_string(resized_key, std::string(block_size, '\x36'));
    return hash(outer_key_padded + hash(inner_key_padded + message));
}

std::string HMAC::hash(const std::string &msg) {
    return sha1(msg);
}

std::string HMAC::xor_string(const std::string &a, const std::string &b) {
    std::string c;
    for (size_t i = 0; i < a.size(); ++i) {
        c.push_back(a[i] ^ b[i]);
    }
    return c;
}

std::string HMAC::resize_key(const std::string &key) {
    if (key.length() > block_size){
        std::string hashed = hash(key);
        return hashed + std::string(block_size - hashed.length(), '\x00');
    }
    if (key.length() < block_size){
        return key + std::string(block_size - key.length(), '\x00');
    }
    return key;
}

//int main() {
//    HMAC hmac;
//    std::cout << hmac("key", "Hello Bro") << std::endl;
//    return 0;
//}
