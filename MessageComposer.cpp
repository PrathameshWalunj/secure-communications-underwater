#include "MessageComposer.h"
#include<iostream>
#include<sstream>
#incldue<openssl/rand.h>

using namespace std;
MessageComposer::MessageComposer() {
    initializeEncryption();
}
MessageComposer::~MessageComposer(){
    cleanupEncryption();
}
void MessageComposer::initializeEncryption(){
    m_encryptionContext = EVP_CIPHER_CTX_new();
    RAND_bytes(m_key, sizeof(m_key));
    RAND_bytes(m_iv,sizeof(m_iv));
}
void MessageComposer::cleanupEncryption(){
    EVP_CIPHER_CTX_free(m_encryptionContext);
}
string MessageComposer::composeMessage(const string& recipeint, const string& subject, const string& content){
    ostringstream messageStream;
    messageStream<<"To: "<<recipeint<<" \n";
    messageStream<<"Subject: "<<subject<<" \n";
    messageStream<<" \n";
    messageStream<<content;
    string message = messageStream.str();
    return message;
}
string MessageComposer::encryptMessage(const string& message){
    EVP_EncryptInit_ex(m_encryptionContext, EVP_aes_256_cbc(), NULL, m_key, m_iv);
    string encryptedMessage;
    int blockSize = EVP_CIPHER_block_size(EVP_aes_256_cbc());
    int encryptedLength = message.length + blockSize;
    encryptedMessage.resize(encryptedLength);
    
    int finalEncryptedLength = 0;
    EVP_EncryptUpdate(m_encryptionContext, reinterpret_cast<unsigned char*>(&encryptedMessage[0]), &encryptedLength, reinterpret_cast<const unsigned char*>(message.c_str()), message.length());
    EVP_EncryptFinal_ex(m_encryptionContext, reinterpret_cast<unsigned char*>(&encryptedMessage[encryptedLength]), &finalEncryptedLength);
 
    encryptedMessage.resize(encryptedLength + finalEncryptedLength);
    string messageHash = calculatemessageHash(message);
    encryptedMessage += messageHash;
    return encryptedMessage;
}
string MessageComposer::calculatemessageHash(const string& message){
    unsigned char hash[SHA3_256_DIGEST_LENGTH];
    SHA3_256(reinterpret_cast<const unsigned char*>(message.c_str()), message.length(), hash);
    string messageHash;
    messageHash.resize(SHA3_256_DIGEST_LENGTH);
    copy(hash, hash + SHA3_256_DIGEST_LENGTH, messageHash.begin());
    return messageHash;
}
