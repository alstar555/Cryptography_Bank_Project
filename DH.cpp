#include "DH.h"

#include "Utils.hpp"
#include <boost/multiprecision/cpp_int.hpp>
//#include <boost/multiprecision/cpp_dec_float.hpp>

DH::DH() {
    initialize_p();
    g = 2;
    local_secret = get_random_int(2048 / 8);
}

void DH::initialize_p() {
    // value from https://www.ietf.org/rfc/rfc3526.txt (GROUP 15)
    std::string value =
        "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1"
        "29024E088A67CC74020BBEA63B139B22514A08798E3404DD"
        "EF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245"
        "E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED"
        "EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3D"
        "C2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F"
        "83655D23DCA3AD961C62F356208552BB9ED529077096966D"
        "670C354E4ABC9804F1746C08CA18217C32905E462E36CE3B"
        "E39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9"
        "DE2BCBF6955817183995497CEA956AE515D2261898FA0510"
        "15728E5A8AAAC42DAD33170D04507A33A85521ABDF1CBA64"
        "ECFB850458DBEF0A8AEA71575D060C7DB3970F85A6E1E4C7"
        "ABF5AE8CDB0933D71E8C94E04A25619DCEE3D2261AD2EE6B"
        "F12FFA06D98A0864D87602733EC86A64521F2B18177B200C"
        "BBE117577A615D6C770988C0BAD946E208E24FA074E5AB31"
        "43DB5BFCE0FD108E4B82D120A93AD2CAFFFFFFFFFFFFFFFF";

    std::stringstream reader;
    reader << std::hex << value;
    reader >> p;
}

boost::multiprecision::cpp_int DH::get_send_value() {
    return boost::multiprecision::powm(g, local_secret, p);
}

void DH::build_shared_secret(boost::multiprecision::cpp_int from_val) {
    shared_secret = boost::multiprecision::powm(from_val, local_secret, p);
}



int main() {
    DH d1;
    DH d2;

    d2.build_shared_secret(d1.get_send_value());
    d1.build_shared_secret(d2.get_send_value());

    std::cout << d1.get_shared_secret() << std::endl;
    std::cout << d2.get_shared_secret() << std::endl;

    std::cout << (d1.get_shared_secret() == d2.get_shared_secret()) << std::endl;
}
