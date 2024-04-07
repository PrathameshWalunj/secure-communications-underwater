#include "MessageComposer.h"
#include<iostream>
#include<sstream>
#include<openssl/rand.h>
#include<openssl/evp.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

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
    int encryptedLength = message.length() + blockSize;
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
    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx, message.c_str(), message.length());
    EVP_DigestFinal_ex(mdctx, hash, NULL);
    EVP_MD_CTX_free(mdctx);
    string messageHash;
    messageHash.resize(SHA256_DIGEST_LENGTH);
    copy(hash, hash + SHA256_DIGEST_LENGTH, messageHash.begin());
    return messageHash;
}
void MessageComposer::sendMessage(const std::string& encryptedMessage) {
    if (encryptedMessage.empty()) {
        std::cerr << "Error! Cannot send an empty message." << std::endl;
        return;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error! Failed to create socket." << std::endl;
        return;
    }

    std::string serverAddress = "192.168.0.10"; // Update with your server address
    int serverPort = 8080; // Update with your server port

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);

    // Convert the server address from string to binary form
    if (inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "Error: Invalid server address." << std::endl;
        close(sockfd);
        return;
    }

    // Connect to the server
    if (connect(sockfd, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
        std::cerr << "Error: Failed to connect to the server." << std::endl;
        close(sockfd);
        return;
    }

    // Send the encrypted message to the server
    ssize_t bytesSent = send(sockfd, encryptedMessage.c_str(), encryptedMessage.length(), 0);
    if (bytesSent < 0) {
        std::cerr << "Error: Failed to send the message." << std::endl;
    } else {
        std::cout << "Message sent successfully." << std::endl;
    }

    // Close the socket
    close(sockfd);
}
