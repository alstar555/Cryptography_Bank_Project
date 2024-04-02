#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <cstdlib>


class RSA{
private:

public:
    uint64_t modinv(uint64_t u, uint64_t v) {
        uint64_t inv, u1, u3, v1, v3, t1, t3, q;
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

    long long encrypt(long long M, long long N, long long e) {
        long long C = 1;
        for (int i = 0; i < e; ++i) {
            C = (C * M) % N;
        }
        return C;
    }


    long long decrypt(long long c, long long d, long long N) {
        long long D = 1;
        for (int i = 0; i < d; i++) {
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
    long long N = 1211;
    long long p = 173;
    long long q = 7;
    long long e = 7;
    long long phi = (p - 1) * (q - 1);
    long long d = rsa.modinv(e, phi);

    std::vector<long long> C2;
    std::vector<long long> D2;

    // Encrypt using RSA
    std::cout << "Cipher text after RSA encryption:" << std::endl;
    for (char c : message) {
        long long cipher = rsa.encrypt(static_cast<long long>(c), N, e);
        C2.push_back(cipher);
        std::cout << cipher << " ";
    }

    // Decrypt RSA
    std::cout << "\nAfter RSA decryption:" << std::endl;
    for (long long c : C2) {
        long long decrypt = rsa.decrypt(c, d, N);
        D2.push_back(decrypt);
        std::cout << static_cast<char>(decrypt) << " ";
    }
    std::cout << std::endl;

    return 0;
}
