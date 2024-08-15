#include "MessageComposer.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include<openssl/err.h>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include<iomanip>

using namespace std;

MessageComposer::MessageComposer() : ctx(EVP_CIPHER_CTX_new()) {
    if (!ctx) {
        throw runtime_error("Failed to create cipher context");
    }
    generateKeyAndIV();
}

MessageComposer::~MessageComposer() {
    EVP_CIPHER_CTX_free(ctx);
}
std::string MessageComposer::getPublicKey() const {
    return key_exchange.getPublicKey();
}


void MessageComposer::generateKeyAndIV() {
    key.resize(32);  // 256-bit key for ChaCha20
    iv.resize(12);   // 96-bit IV for ChaCha20
    if (RAND_bytes(key.data(), key.size()) != 1 ||
        RAND_bytes(iv.data(), iv.size()) != 1) {
        throw runtime_error("Failed to generate key or IV");
    }
}

string MessageComposer::composeMessage(const string& recipient, const string& subject, const string& content) {
    ostringstream oss;
    oss << "To: " << recipient << "\n";
    oss << "Subject: " << subject << "\n\n";
    oss << content;
    string composed = oss.str();
    
    cout << "Composed message:" << endl;
    cout << composed << endl;
    cout << "Message length: " << composed.length() << endl;
    
    return composed;
}



std::string MessageComposer::encryptMessage(const std::string& message, const std::string& recipient_public_key) {
    // Generate a new random key for this message
    key.resize(32);  // 256-bit key for ChaCha20
    if (RAND_bytes(key.data(), key.size()) != 1) {
        throw std::runtime_error("Failed to generate key");
    }

    // Encrypt the message
    std::vector<unsigned char> ciphertext(message.size() + 16);  // Extra space for tag
    int len, ciphertext_len;

    if (EVP_EncryptInit_ex(ctx, EVP_chacha20_poly1305(), nullptr, key.data(), iv.data()) != 1) {
        throw std::runtime_error("Failed to initialize encryption");
    }

    if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, 
                          reinterpret_cast<const unsigned char*>(message.data()), message.size()) != 1) {
        throw std::runtime_error("Failed to encrypt message");
    }
    ciphertext_len = len;

    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1) {
        throw std::runtime_error("Failed to finalize encryption");
    }
    ciphertext_len += len;

    ciphertext.resize(ciphertext_len);
    
    // Prepend IV to ciphertext
    ciphertext.insert(ciphertext.begin(), iv.begin(), iv.end());

    // Encrypt the symmetric key with the recipient's public key
    std::vector<unsigned char> encrypted_key = key_exchange.encryptKey(key, recipient_public_key);

    // Combine encrypted key and ciphertext
    std::string result = base64Encode(encrypted_key) + ":" + base64Encode(ciphertext);

    return result;
}



string MessageComposer::decryptMessage(const string& encryptedMessage, const std::vector<unsigned char>& encrypted_key)  {
    cout << "Attempting to decrypt message: " << encryptedMessage << endl;
    
    // Remove any non-Base64 characters from the input
    string cleanedMessage = encryptedMessage;
    cleanedMessage.erase(remove_if(cleanedMessage.begin(), cleanedMessage.end(), 
                         [](unsigned char c) { return !isalnum(c) && c != '+' && c != '/'; }),
                         cleanedMessage.end());
    
    // Split the input into encrypted key and ciphertext
    size_t colonPos = cleanedMessage.find(':');
    if (colonPos == string::npos) {
        throw runtime_error("Invalid input format");
    }

    string encryptedKeyStr = cleanedMessage.substr(0, colonPos);
    string ciphertextStr = cleanedMessage.substr(colonPos + 1);

    // Add padding if necessary
    while (ciphertextStr.length() % 4 != 0) {
        ciphertextStr += '=';
    }
    
    cout << "Cleaned message for Base64 decoding: " << ciphertextStr << endl;
    
    // Decrypt the symmetric key
    std::vector<unsigned char> decrypted_key = key_exchange.decryptKey(encrypted_key);

    // Use the decrypted key to decrypt the message
    std::vector<unsigned char> ciphertext = base64Decode(ciphertextStr);

    cout << "Base64 decoding successful. Decoded length: " << ciphertext.size() << endl;

    if (ciphertext.size() <= 12) {  // 12 is the IV size
        throw runtime_error("Encrypted message is too short");
    }

    // Extract IV from ciphertext
    vector<unsigned char> message_iv(ciphertext.begin(), ciphertext.begin() + 12);
    ciphertext.erase(ciphertext.begin(), ciphertext.begin() + 12);

    vector<unsigned char> plaintext(ciphertext.size());
    int len, plaintext_len;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw runtime_error("Failed to create cipher context");
    }

    if (EVP_DecryptInit_ex(ctx, EVP_chacha20_poly1305(), nullptr, decrypted_key.data(), message_iv.data()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw runtime_error("Failed to initialize decryption");
    }

    if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw runtime_error("Failed to decrypt message");
    }
    plaintext_len = len;

    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw runtime_error("Failed to finalize decryption");
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    string decryptedMessage(plaintext.begin(), plaintext.begin() + plaintext_len);
    
    cout << "Decryption successful. Decrypted message (hex): ";
    for (unsigned char c : decryptedMessage) {
        cout << setw(2) << setfill('0') << hex << static_cast<int>(c) << " ";
    }
    cout << endl;

    cout << "Decrypted message (printable chars): ";
    for (unsigned char c : decryptedMessage) {
        if (isprint(c)) {
            cout << c;
        } else {
            cout << ".";
        }
    }
    cout << endl;

    return decryptedMessage;
}


string MessageComposer::base64Encode(const vector<unsigned char>& input) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, input.data(), input.size());
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);

    string result(bufferPtr->data, bufferPtr->length);

    BIO_free_all(bio);

    return result;
}

vector<unsigned char> MessageComposer::base64Decode(const string& input) {
    BIO *bio, *b64;
    vector<unsigned char> result(input.size());

    b64 = BIO_new(BIO_f_base64());
    if (!b64) {
        throw runtime_error("Failed to create Base64 BIO");
    }

    bio = BIO_new_mem_buf(input.data(), input.size());
    if (!bio) {
        BIO_free_all(b64);
        throw runtime_error("Failed to create memory BIO");
    }

    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    int decoded_size = BIO_read(bio, result.data(), input.size());
    if (decoded_size < 0) {
        BIO_free_all(bio);
        unsigned long err = ERR_get_error();
        char err_msg[256];
        ERR_error_string_n(err, err_msg, sizeof(err_msg));
        throw runtime_error("Failed to decode Base64 input: " + string(err_msg));
    }

    BIO_free_all(bio);

    if (decoded_size == 0) {
        cout << "Warning: No data read during Base64 decoding. Input might be empty or invalid." << endl;
    }

    result.resize(decoded_size);
    return result;
}
