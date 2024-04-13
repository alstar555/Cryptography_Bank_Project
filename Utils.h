#ifndef CRYPTOGRAPHY_BANK_PROJECT_UTILS_H
#define CRYPTOGRAPHY_BANK_PROJECT_UTILS_H

#include <vector>
#include <boost/multiprecision/cpp_int.hpp>

std::vector<uint8_t> get_random_bytes(unsigned int num_bytes);

boost::multiprecision::cpp_int get_random_int(unsigned int num_bytes);

boost::multiprecision::cpp_int get_random_prime(unsigned int num_bytes);

std::string convert_byte_str_to_hex(const std::string& byte_str);

boost::multiprecision::cpp_int int_from_bytes(const std::vector<uint8_t>& bytes);

std::vector<uint8_t> bytes_from_int(const boost::multiprecision::cpp_int& cpp_int);

boost::multiprecision::cpp_int int_from_longs(const std::vector<unsigned long>& longs);

std::vector<unsigned long> longs_from_int(const boost::multiprecision::cpp_int& cpp_int);

unsigned long random_iv();

#endif //CRYPTOGRAPHY_BANK_PROJECT_UTILS_H
