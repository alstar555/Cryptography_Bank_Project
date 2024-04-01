#include "DES.h"
#include <iostream>

unsigned long DES::encrypt(unsigned long plaintext, unsigned long key) {
    unsigned long tmp = initial_perm(plaintext);

    // go through 16 rounds
    for (int i = 0; i < 16; i++) {
        unsigned long round_key = key_schedule(key, i+1);

        unsigned long left = tmp >> 32;
        unsigned long right = tmp & ((1ul << 32) - 1);

        tmp = do_round(left, right, round_key);

        if (i != 15) {
            // flip left and right
            left = tmp >> 32;
            right = tmp & ((1ul << 32) - 1);

            tmp = (right << 32) | left;
        }
    }

    return inverse_initial_perm(tmp);
}

unsigned long DES::decrypt(unsigned long ciphertext, unsigned long key) {
    unsigned long tmp = initial_perm(ciphertext);

    // go through 16 rounds
    for (int i = 15; i >= 0; i--) {
        unsigned long round_key = key_schedule(key, i+1);

        unsigned long left = tmp >> 32;
        unsigned long right = tmp & ((1ul << 32) - 1);

        tmp = do_round(left, right, round_key);

        if (i != 0) {
            // flip left and right
            left = tmp >> 32;
            right = tmp & ((1ul << 32) - 1);

            tmp = (right << 32) | left;
        }
    }

    return inverse_initial_perm(tmp);
}

unsigned long DES::encrypt3(unsigned long plaintext, unsigned long key1, unsigned long key2, unsigned long key3) {
    unsigned long ret = encrypt(plaintext, key1);
    ret = decrypt(ret, key2);
    ret = encrypt(ret, key3);
    return ret;
}

unsigned long DES::decrypt3(unsigned long ciphertext, unsigned long key1, unsigned long key2, unsigned long key3) {
    unsigned long ret = decrypt(ciphertext, key3);
    ret = encrypt(ret, key2);
    ret = decrypt(ret, key1);
    return ret;
}

std::vector<unsigned long>
DES::encrypt3_cbc(const std::vector<unsigned long> &plaintext, unsigned long key1, unsigned long key2, unsigned long key3, unsigned long iv) {
    std::vector<unsigned long> ret;
    unsigned long prev = iv;

    for (unsigned long val : plaintext) {
        unsigned long tmp = encrypt3(prev ^ val, key1, key2, key3);
        ret.push_back(tmp);
        prev = tmp;
    }

    return ret;
}

std::vector<unsigned long>
DES::decrypt3_cbc(const std::vector<unsigned long> &ciphertext, unsigned long key1, unsigned long key2, unsigned long key3, unsigned long iv) {
    std::vector<unsigned long> ret;

    if (!ciphertext.empty()) {
        ret.push_back(decrypt3(ciphertext[0], key1, key2, key3) ^ iv);
    }
    for (int i = 1; i < ciphertext.size(); i++) {
        ret.push_back(decrypt3(ciphertext[i], key1, key2, key3) ^ ciphertext[i - 1]);
    }

    return ret;
}

unsigned long DES::do_round(unsigned long left_side, unsigned long right_side, unsigned long round_key) {
    unsigned long ret = right_side;
    ret |= (f_function(right_side, round_key) ^ left_side) << 32;
    return ret;
}

unsigned long DES::f_function(unsigned long input, unsigned long round_key) {
    unsigned long expanded = expansion(input);
    unsigned long xor_result = expanded ^ round_key;
    unsigned long sbox_res = full_s_box(xor_result);
    return f_function_perm(sbox_res);
}

unsigned long DES::f_function_perm(unsigned long input) {
    unsigned long ret = 0;
    unsigned int selection_table[8][4] = {
        {16, 7, 20, 21},
        {29, 12, 28, 17},
        {1, 15, 23, 26},
        {5, 18, 31, 10},
        {2, 8, 24, 14},
        {32, 27, 3, 9},
        {19, 13, 30, 6},
        {22, 11, 4, 25}
    };

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            unsigned int target_position_bit = i * 4 + j + 1;
            unsigned int from_position_bit = selection_table[i][j];

            ret |= ((input >> (32 - from_position_bit)) & 1) << (32 - target_position_bit);
        }
    }

    return ret;
}

unsigned long DES::single_s_box_get(unsigned long input, unsigned long box) {
    unsigned long boxes[8][4][16] = {
        {
            {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
            {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
            {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
            {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
        },
        {
            {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
            {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
            {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
            {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
        },
        {
            {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
            {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
            {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
            {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
        },
        {
            {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
            {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
            {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
            {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
        },
        {
            {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
            {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
            {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
            {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
        },
        {
            {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
            {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
            {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
            {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
        },
        {
            {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
            {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
            {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
            {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
        },
        {
            {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
            {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
            {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
            {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
        }
    };

    unsigned int i = input & 1;
    i |= (input >> 5) << 1;
    unsigned int j = (input >> 1) & ((1 << 4) - 1);

    return boxes[box][i][j];
}

unsigned long DES::full_s_box(unsigned long input) {
    unsigned long ret = 0;
    for (int i = 0; i < 8; i++) {
        // get each 6-bit segment
        unsigned long bit_segment = (input >> (6 * (8-i-1))) & ((1 << 6) - 1);
        unsigned long val = single_s_box_get(bit_segment, i);
        ret |= single_s_box_get(bit_segment, i) << ((8-i-1)*4);
    }
    return ret;
}

unsigned long DES::expansion(unsigned long input) {
    unsigned long ret = 0;
    unsigned int selection_table[8][6] = {
        {32, 1, 2, 3, 4, 5},
        {4, 5, 6, 7, 8, 9},
        {8, 9, 10, 11, 12, 13},
        {12, 13, 14, 15, 16, 17},
        {16, 17, 18, 19, 20, 21},
        {20, 21, 22, 23, 24, 25},
        {24, 25, 26, 27, 28, 29},
        {28, 29, 30, 31, 32, 1}
    };

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 6; j++) {
            unsigned int target_position_bit = i * 6 + j + 1;
            unsigned int from_position_bit = selection_table[i][j];

            ret |= ((input >> (32 - from_position_bit)) & 1) << (48 - target_position_bit);
        }
    }

    return ret;
}

unsigned long DES::initial_perm(unsigned long input) {
    unsigned long ret = 0;
    unsigned int selection_table[8][8] = {
        {58, 50, 42, 34, 26, 18, 10, 2},
        {60, 52, 44, 36, 28, 20, 12, 4},
        {62, 54, 46, 38, 30, 22, 14, 6},
        {64, 56, 48, 40, 32, 24, 16, 8},
        {57, 49, 41, 33, 25, 17, 9, 1},
        {59, 51, 43, 35, 27, 19, 11, 3},
        {61, 53, 45, 37, 29, 21, 13, 5},
        {63, 55, 47, 39, 31, 23, 15, 7}
    };

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            unsigned int target_position_bit = i * 8 + j + 1;
            unsigned int from_position_bit = selection_table[i][j];

            ret |= ((input >> (64 - from_position_bit)) & 1) << (64 - target_position_bit);
        }
    }

    return ret;
}

unsigned long DES::inverse_initial_perm(unsigned long input) {
    unsigned long ret = 0;
    unsigned int selection_table[8][8] = {
        {40, 8, 48, 16, 56, 24, 64, 32},
        {39, 7, 47, 15, 55, 23, 63, 31},
        {38, 6, 46, 14, 54, 22, 62, 30},
        {37, 5, 45, 13, 53, 21, 61, 29},
        {36, 4, 44, 12, 52, 20, 60, 28},
        {35, 3, 43, 11, 51, 19, 59, 27},
        {34, 2, 42, 10, 50, 18, 58, 26},
        {33, 1, 41, 9, 49, 17, 57, 25}
    };

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            unsigned int target_position_bit = i * 8 + j + 1;
            unsigned int from_position_bit = selection_table[i][j];

            ret |= ((input >> (64 - from_position_bit)) & 1) << (64 - target_position_bit);
        }
    }

    return ret;
}

unsigned long left_circ_shift(unsigned long in) {
    // store the left most bit
    unsigned long left_most_bit = (in >> 27) & 1;
    // shift the input by 1 and AND off the rest
    in = (in << 1) & ((1 << 28) - 1);
    // put the left most bit on the right
    in = in | left_most_bit;
    return in;
}

// round must start at 1 (NOT 0!!!)
unsigned long DES::key_schedule(unsigned long key, int round) {
    unsigned long tmp_key = key_permuted_choice_1(key);
    unsigned long key_parts[2];
    key_parts[0] = tmp_key >> 28;
    key_parts[1] = tmp_key & ((1 << 28) - 1);

    int shifts_per_round[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

    for (int i = 0; i < round; i++) {
        for (int j = 0; j < shifts_per_round[i]; j++) {
            key_parts[0] = left_circ_shift(key_parts[0]);
            key_parts[1] = left_circ_shift(key_parts[1]);
        }
    }

    // create normal integer from parts
    unsigned long ret = (key_parts[0] << 28) | key_parts[1];
    ret = key_permuted_choice_2(ret);

    return ret;
}

unsigned long DES::key_permuted_choice_1(unsigned long input) {
    unsigned long ret = 0;
    unsigned int selection_table[8][7] = {
        {57, 49, 41, 33, 25, 17, 9},
        {1, 58, 50, 42, 34, 26, 18},
        {10, 2, 59, 51, 43, 35, 27},
        {19, 11, 3, 60, 52, 44, 36},
        {63, 55, 47, 39, 31, 23, 15},
        {7, 62, 54, 46, 38, 30, 22},
        {14, 6, 61, 53, 45, 37, 29},
        {21, 13, 5, 28, 20, 12, 4}
    };

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 7; j++) {
            unsigned int target_position_bit = i * 7 + j + 1;
            unsigned int from_position_bit = selection_table[i][j];

            ret |= ((input >> (64 - from_position_bit)) & 1) << (56 - target_position_bit);
        }
    }

    return ret;
}

unsigned long DES::key_permuted_choice_2(unsigned long input) {
    unsigned long ret = 0;
    unsigned int selection_table[8][6] = {
        {14, 17, 11, 24, 1, 5},
        {3, 28, 15, 6, 21, 10},
        {23, 19, 12, 4, 26, 8},
        {16, 7, 27, 20, 13, 2},
        {41, 52, 31, 37, 47, 55},
        {30, 40, 51, 45, 33, 48},
        {44, 49, 39, 56, 34, 53},
        {46, 42, 50, 36, 29, 32}
    };

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 6; j++) {
            unsigned int target_position_bit = i * 6 + j + 1;
            unsigned int from_position_bit = selection_table[i][j];

            ret |= ((input >> (56 - from_position_bit)) & 1) << (48 - target_position_bit);
        }
    }

    return ret;
}

int main() {
    unsigned long ptxt = 84903248ul;
    unsigned long key = 1023040812745559411ul;

    unsigned long res = DES::encrypt3(ptxt, key, key, key);

    std::cout << res << std::endl;

    res = DES::decrypt3(res, key, key, key);
    std::cout << res << std::endl;
}
