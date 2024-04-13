#ifndef CRYPTOGRAPHY_BANK_PROJECT_CLIENT_H
#define CRYPTOGRAPHY_BANK_PROJECT_CLIENT_H

#include "crypto/DH.h"
#include "crypto/DESKey.h"

class Client {
private:
    DH diffie_hellman;
    DESKey des_key;
    std::string hmac_key;

public:
    void build_des_key_from_dh();

    DH& getDH() { return diffie_hellman; };

    const DESKey& getDESKey() const { return des_key; }

    void setHMACKey(std::string key) { hmac_key = key; }

    const std::string& getHMACKey() const { return hmac_key; }
};


#endif //CRYPTOGRAPHY_BANK_PROJECT_CLIENT_H
