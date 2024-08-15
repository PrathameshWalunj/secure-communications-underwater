#ifndef MESSAGE_COMPOSER_H
#define MESSAGE_COMPOSER_H
#include "KeyExchange.h"
#include <string>
#include <vector>
#include <openssl/evp.h>

using namespace std;

class MessageComposer {
public:
    MessageComposer();
    ~MessageComposer();
    std::string getPublicKey() const;
    std::string encryptMessage(const std::string& message, const std::string& recipient_public_key);
    std::string decryptMessage(const std::string& encrypted_message, const std::vector<unsigned char>& encrypted_key);
    string composeMessage(const string& recipient, const string& subject, const string& content);
    vector<unsigned char> base64Decode(const string& input);
    

private:
    EVP_CIPHER_CTX *ctx;
    vector<unsigned char> key;
    vector<unsigned char> iv;
    KeyExchange key_exchange;

    void generateKeyAndIV();
    string base64Encode(const vector<unsigned char>& input);
    
};

#endif 