#ifndef CRYPTOGRAPHY_BANK_PROJECT_UTILS_H
#define CRYPTOGRAPHY_BANK_PROJECT_UTILS_H

#include <vector>
#include <boost/multiprecision/cpp_int.hpp>

std::vector<uint8_t> get_random_bytes(unsigned int num_bytes);

boost::multiprecision::cpp_int get_random_int(unsigned int num_bytes);

boost::multiprecision::cpp_int get_random_prime(unsigned int num_bytes);

std::string convert_byte_str_to_hex(const std::string& byte_str);

#endif //CRYPTOGRAPHY_BANK_PROJECT_UTILS_H
