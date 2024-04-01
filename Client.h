#ifndef CRYPTOGRAPHY_BANK_PROJECT_CLIENT_H
#define CRYPTOGRAPHY_BANK_PROJECT_CLIENT_H

#include "DH.h"
#include "DESKey.h"

class Client {
private:
    DH diffie_hellman;
    DESKey des_key;

public:
    void build_des_key_from_dh();

    DH& getDH() { return diffie_hellman; };

    const DESKey& getDESKey() const { return des_key; }
};


#endif //CRYPTOGRAPHY_BANK_PROJECT_CLIENT_H
