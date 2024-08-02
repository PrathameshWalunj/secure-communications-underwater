#ifndef MESSAGE_COMPOSER_H
#define MESSAGE_COMPOSER_H

#include <string>
#include <vector>
#include <openssl/evp.h>

using namespace std;

class MessageComposer {
public:
    MessageComposer();
    ~MessageComposer();
    string composeMessage(const string& recipient, const string& subject, const string& content);
    string encryptMessage(const string& message);
    string decryptMessage(const string& encryptedMessage);

private:
    EVP_CIPHER_CTX *ctx;
    vector<unsigned char> key;
    vector<unsigned char> iv;

    void generateKeyAndIV();
    string base64Encode(const vector<unsigned char>& input);
    vector<unsigned char> base64Decode(const string& input);
};

#endif 