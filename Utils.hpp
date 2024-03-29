#ifndef CRYPTOGRAPHY_BANK_PROJECT_UTILS_HPP
#define CRYPTOGRAPHY_BANK_PROJECT_UTILS_HPP

#include <vector>
#include <fstream>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/miller_rabin.hpp>

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

boost::multiprecision::cpp_int get_random_prime(unsigned int num_bytes) {
    while (true) {
        auto potential_prime = get_random_int(num_bytes);
        if (boost::multiprecision::miller_rabin_test(potential_prime, 40)) {
            return potential_prime;
        }
    }
}

#endif //CRYPTOGRAPHY_BANK_PROJECT_UTILS_HPP
