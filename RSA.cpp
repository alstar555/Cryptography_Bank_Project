#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "Utils.hpp"
#include <boost/multiprecision/cpp_int.hpp>

class RSA {
public:
    using cpp_int = boost::multiprecision::cpp_int;

    cpp_int modinv(cpp_int u, cpp_int v) {
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

    cpp_int encrypt(cpp_int M, cpp_int N, cpp_int e) {
        cpp_int C = 1;
        for (cpp_int i = 0; i < e; ++i) {
            C = (C * M) % N;
        }
        return C;
    }

    cpp_int decrypt(cpp_int c, cpp_int d, cpp_int N) {
        cpp_int D = 1;
        for (cpp_int i = 0; i < d; i++) {
            D = (D * c) % N;
        }
        return D;
    }
};

int main() {
    RSA rsa;

    std::string message = "NETSEC";
    std::cout << "Original message:\n" << message << std::endl;
    std::cout << std::endl;

    // RSA vars
    RSA::cpp_int p = get_random_prime(2); 
    RSA::cpp_int q = get_random_prime(2);
    std::cout << "p: " << p << " q: " << q << std::endl;
    RSA::cpp_int N = p * q;
    RSA::cpp_int e = 7;
    RSA::cpp_int phi = (p - 1) * (q - 1);
    RSA::cpp_int d = rsa.modinv(e, phi);

    std::vector<RSA::cpp_int> C2;
    std::vector<RSA::cpp_int> D2;

    // Encrypt using RSA
    std::cout << "Cipher text after RSA encryption:" << std::endl;
    for (char c : message) {
        RSA::cpp_int cipher = rsa.encrypt(static_cast<RSA::cpp_int>(c), N, e);
        C2.push_back(cipher);
        std::cout << cipher << " ";
    }

    // Decrypt RSA
    std::cout << "\nAfter RSA decryption:" << std::endl;
    for (RSA::cpp_int c : C2) {
        RSA::cpp_int decrypt = rsa.decrypt(c, d, N);
        D2.push_back(decrypt);
        std::cout << static_cast<char>(decrypt) << " ";
    }
    std::cout << std::endl;

    return 0;
}
