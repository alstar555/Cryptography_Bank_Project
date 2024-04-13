#ifndef CRYPTOGRAPHY_BANK_PROJECT_RSA_H
#define CRYPTOGRAPHY_BANK_PROJECT_RSA_H

#include <boost/multiprecision/cpp_int.hpp>
#include "RSAPrivateKey.h"
#include "RSAPublicKey.h"

class RSA {
public:
    using cpp_int = boost::multiprecision::cpp_int;

    cpp_int modinv(cpp_int u, cpp_int v);
    cpp_int encrypt(cpp_int M, cpp_int N, cpp_int e);
    cpp_int decrypt(cpp_int c, cpp_int d, cpp_int N);

    static RSAPrivateKey parsePrivateKey(std::istream& istream);
    static RSAPublicKey parsePublicKey(std::istream& istream);
};

#endif //CRYPTOGRAPHY_BANK_PROJECT_RSA_H
