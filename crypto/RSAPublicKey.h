#ifndef CRYPTOGRAPHY_BANK_PROJECT_RSAPUBLICKEY_H
#define CRYPTOGRAPHY_BANK_PROJECT_RSAPUBLICKEY_H

#include <boost/multiprecision/cpp_int.hpp>

class RSAPublicKey {
private:
    boost::multiprecision::cpp_int n;
    boost::multiprecision::cpp_int e;

public:
    RSAPublicKey() = default;

    RSAPublicKey(
            boost::multiprecision::cpp_int in_n,
            boost::multiprecision::cpp_int in_e
            ) : n(in_n), e(in_e) {}

    boost::multiprecision::cpp_int getN() const { return n; }
    boost::multiprecision::cpp_int getE() const { return e; }
};

#endif //CRYPTOGRAPHY_BANK_PROJECT_RSAPUBLICKEY_H
