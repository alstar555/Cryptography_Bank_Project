#ifndef CRYPTOGRAPHY_BANK_PROJECT_DESKEY_H
#define CRYPTOGRAPHY_BANK_PROJECT_DESKEY_H


class DESKey {
private:
    unsigned long key1;
    unsigned long key2;
    unsigned long key3;

public:
    DESKey();
    DESKey(unsigned long provided_key1, unsigned long provided_key2, unsigned long provided_key3);

    const unsigned long& getKey1() const { return key1; }
    const unsigned long& getKey2() const { return key2; }
    const unsigned long& getKey3() const { return key3; }
};


#endif //CRYPTOGRAPHY_BANK_PROJECT_DESKEY_H
