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

std::string convert_byte_str_to_hex(const std::string& byte_str) {
    std::stringstream ss;
    for (char c : byte_str) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)(uint8_t)c;
    }
    return ss.str();
}

boost::multiprecision::cpp_int int_from_bytes(const std::vector<uint8_t>& bytes) {
    boost::multiprecision::cpp_int ret;
    boost::multiprecision::import_bits(ret, bytes.begin(), bytes.end());
    return ret;
}

std::vector<uint8_t> bytes_from_int(const boost::multiprecision::cpp_int& cpp_int) {
    std::vector<uint8_t> ret;
    boost::multiprecision::export_bits(cpp_int, std::back_inserter(ret), 8);
    return ret;
}

boost::multiprecision::cpp_int int_from_longs(const std::vector<unsigned long>& longs) {
    boost::multiprecision::cpp_int ret;
    boost::multiprecision::import_bits(ret, longs.begin(), longs.end(), 64);
    return ret;
}

std::vector<unsigned long> longs_from_int(const boost::multiprecision::cpp_int& cpp_int) {
    std::vector<unsigned long> ret;
    boost::multiprecision::export_bits(cpp_int, std::back_inserter(ret), 64);
    return ret;
}

unsigned long random_iv() {
    auto random_int = get_random_int(8);
    return random_int.convert_to<unsigned long>();
}
