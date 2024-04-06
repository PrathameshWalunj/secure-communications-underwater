#ifndef MESSAGECOMPOSER_H
#define MESSAGECOMPOSER_H

#include<string>
#include<openssl/evp.h>
#include<openssl/sha.h>
using namespace std;

class MessageComposer {
    public: 
        MessageComposer();
        ~MessageComposer();
        string composeMessage(const string& recipient, const string& subject, const string& content);
        string encryptMessage(const string& message);
        void sendMessage(const string&encrpytedMessage);

    private:
        EVP_CIPHER_CTX* m_encryptionContext;
        unsigned char m_key[32];
        unsigned char m_iv[16];
        void initializeEncryption();
        void cleanupEncryption();
        string calculatemessageHash(const string& message);
        
        };

    #endif
