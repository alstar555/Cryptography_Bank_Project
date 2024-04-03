#ifndef CRYPTOGRAPHY_BANK_PROJECT_DES_H
#define CRYPTOGRAPHY_BANK_PROJECT_DES_H

#include <vector>
#include "DESKey.h"

class DES {
private:
    // left_side will be XOR'd against result of F function
    // right_side will be passed into the F function
    // the resulting combined and XOR'd round result is returned
    static unsigned long do_round(unsigned long left_side, unsigned long right_side, unsigned long round_key);

    // inputs 32 bit integer and outputs 48 bit integer
    static unsigned long expansion(unsigned long input);

    static unsigned long f_function(unsigned long input, unsigned long round_key);

    static unsigned long full_s_box(unsigned long input);

    // input is 6 bit number
    // box is which s-box to use
    static unsigned long single_s_box_get(unsigned long input, unsigned long box);

    // input is 32 bit integer and returns 32 bit integer
    static unsigned long f_function_perm(unsigned long input);

    static unsigned long initial_perm(unsigned long input);

    static unsigned long inverse_initial_perm(unsigned long input);

    static unsigned long key_schedule(unsigned long key, int round);

    static unsigned long key_permuted_choice_1(unsigned long input);
    static unsigned long key_permuted_choice_2(unsigned long input);
public:
    // Regular DES below

    // Accepts 64 bit plaintext and 56 bit key
    static unsigned long encrypt(unsigned long plaintext, unsigned long key);

    // Accepts 64 bit ciphertext and 56 bit key
    static unsigned long decrypt(unsigned long ciphertext, unsigned long key);

    // Triple DES below

    static unsigned long encrypt3(unsigned long plaintext, const DESKey& key);

    static unsigned long decrypt3(unsigned long ciphertext, const DESKey& key);

    static std::vector<unsigned long> encrypt3_cbc(const std::vector<unsigned long>& plaintext, const DESKey& key, unsigned long iv);

    static std::vector<unsigned long> decrypt3_cbc(const std::vector<unsigned long>& ciphertext, const DESKey& key, unsigned long iv);
};

#endif //CRYPTOGRAPHY_BANK_PROJECT_DES_H
