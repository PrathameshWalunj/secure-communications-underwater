#ifndef KEY_EXCHANGE_H
#define KEY_EXCHANGE_H

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <string>
#include <vector>

class KeyExchange {
private:
    RSA* rsa;
    std::string public_key;
    std::string private_key;

public:
    KeyExchange();
    ~KeyExchange();

    std::string getPublicKey() const;
    std::vector<unsigned char> encryptKey(const std::vector<unsigned char>& key, const std::string& recipient_public_key);
    std::vector<unsigned char> decryptKey(const std::vector<unsigned char>& encrypted_key);
};

#endif