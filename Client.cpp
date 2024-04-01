#include "Client.h"
#include <boost/multiprecision/cpp_int.hpp>

void Client::build_des_key_from_dh() {
    auto shared_secret = diffie_hellman.get_shared_secret();
    boost::multiprecision::cpp_int one = 1;
    boost::multiprecision::cpp_int tmp = shared_secret & ((one << 64) - 1);
    unsigned long key1 = tmp.convert_to<unsigned long>();
    tmp = shared_secret >> 64;
    tmp = tmp & ((one << 64) - 1);
    unsigned long key2 = tmp.convert_to<unsigned long>();
    tmp = shared_secret >> 128;
    tmp = tmp & ((one << 64) - 1);
    unsigned long key3 = tmp.convert_to<unsigned long>();
    des_key = DESKey(key1, key2, key3);
}
