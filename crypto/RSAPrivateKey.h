#ifndef CRYPTOGRAPHY_BANK_PROJECT_RSA_PRIVATE_KEY_H
#define CRYPTOGRAPHY_BANK_PROJECT_RSA_PRIVATE_KEY_H

#include <boost/multiprecision/cpp_int.hpp>

class RSAPrivateKey {
private:
    boost::multiprecision::cpp_int p;
    boost::multiprecision::cpp_int q;
    boost::multiprecision::cpp_int d;
    boost::multiprecision::cpp_int n;

public:
    RSAPrivateKey() = default;

    RSAPrivateKey(
            boost::multiprecision::cpp_int in_p,
            boost::multiprecision::cpp_int in_q,
            boost::multiprecision::cpp_int in_d
            ) : p(in_p), q(in_q), d(in_d) { n = p * q; }

    boost::multiprecision::cpp_int getP() const { return p; }
    boost::multiprecision::cpp_int getQ() const { return q; }
    boost::multiprecision::cpp_int getD() const { return d; }
    boost::multiprecision::cpp_int getN() const { return n; }

};


#endif //CRYPTOGRAPHY_BANK_PROJECT_RSA_PRIVATE_KEY_H
