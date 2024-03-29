#ifndef CRYPTOGRAPHY_BANK_PROJECT_UTILS_HPP
#define CRYPTOGRAPHY_BANK_PROJECT_UTILS_HPP

#include <vector>
#include <fstream>
#include <boost/multiprecision/cpp_int.hpp>

std::vector<uint8_t> get_random_bytes(unsigned int num_bytes) {
    auto result = new uint8_t[num_bytes];
    std::ifstream r("/dev/urandom", std::ios_base::binary);
    r.read(reinterpret_cast<char *>(result), num_bytes);
    std::vector<uint8_t> ret;
    ret.assign(result, result + num_bytes);
    delete [] result;
    return ret;
}

boost::multiprecision::cpp_int get_random_int(unsigned int num_bytes) {
    auto bytes = get_random_bytes(num_bytes);
    boost::multiprecision::cpp_int ret;
    boost::multiprecision::import_bits(ret, bytes.begin(), bytes.end());
    return ret;
}

#endif //CRYPTOGRAPHY_BANK_PROJECT_UTILS_HPP
