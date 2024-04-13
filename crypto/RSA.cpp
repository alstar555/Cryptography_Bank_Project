#include <cstdlib>
#include "RSA.h"
#include <boost/multiprecision/cpp_int.hpp>
#include "../Utils.h"

using cpp_int = boost::multiprecision::cpp_int;

cpp_int RSA::modinv(cpp_int u, cpp_int v) {
    cpp_int inv, u1, u3, v1, v3, t1, t3, q;
    int64_t iter;
    u1 = 1;
    u3 = u;
    v1 = 0;
    v3 = v;
    iter = 1;
    while (v3 != 0) {
        q = u3 / v3;
        t3 = u3 % v3;
        t1 = u1 + q * v1;
        u1 = v1;
        v1 = t1;
        u3 = v3;
        v3 = t3;
        iter = -iter;
    }
    if (u3 != 1)
        return  0;
    if (iter < 0)
        inv = v - u1;
    else
        inv = u1;
    return inv;
}

cpp_int RSA::encrypt(cpp_int M, cpp_int N, cpp_int e) {
    return boost::multiprecision::powm(M, e, N);
}

cpp_int RSA::decrypt(cpp_int c, cpp_int d, cpp_int N) {
    return boost::multiprecision::powm(c, d, N);
}

RSAPrivateKey RSA::parsePrivateKey(std::istream &istream) {
    std::string val;
    istream >> val;
    cpp_int p(val);
    istream >> val;
    cpp_int q(val);
    istream >> val;
    cpp_int d(val);
    return RSAPrivateKey(p, q, d);
}

RSAPublicKey RSA::parsePublicKey(std::istream &istream) {
    std::string val;
    istream >> val;
    cpp_int n(val);
    istream >> val;
    cpp_int e(val);
    return RSAPublicKey(n, e);
}

//int main() {
//    RSA rsa;
//
//    std::string message = "NETSEC";
//    std::cout << "Original message:\n" << message << std::endl;
//    std::cout << std::endl;
//
//    // RSA vars
//    RSA::cpp_int p = get_random_prime(256);
//    RSA::cpp_int q = get_random_prime(256);
//    std::cout << "p: " << p << "\nq: " << q << std::endl;
//    RSA::cpp_int N = p * q;
//    RSA::cpp_int e = 65537;
//    RSA::cpp_int phi = (p - 1) * (q - 1);
//    RSA::cpp_int d = rsa.modinv(e, phi);
//
//    std::cout << "d: " << d << std::endl;
//    std::cout << "e: " << e << std::endl;
//    std::cout << "n: " << N << std::endl;
//
//    std::vector<RSA::cpp_int> C2;
//    std::vector<RSA::cpp_int> D2;
//
//    // Encrypt using RSA
//    std::cout << "Cipher text after RSA encryption:" << std::endl;
//    for (char c : message) {
//        RSA::cpp_int cipher = rsa.encrypt(static_cast<RSA::cpp_int>(c), N, e);
//        C2.push_back(cipher);
//        std::cout << cipher << " ";
//    }
//
//    // Decrypt RSA
//    std::cout << "\nAfter RSA decryption:" << std::endl;
//    for (RSA::cpp_int c : C2) {
//        RSA::cpp_int decrypt = rsa.decrypt(c, d, N);
//        D2.push_back(decrypt);
//        std::cout << static_cast<char>(decrypt) << " ";
//    }
//    std::cout << std::endl;
//
//    return 0;
//}
