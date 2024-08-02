#include "MessageComposer.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <stdexcept>
#include <sstream>
#include <iostream>

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
    return oss.str();
}

string MessageComposer::encryptMessage(const string& message) {
    vector<unsigned char> ciphertext(message.size() + 16);  // Extra space for tag
    int len, ciphertext_len;

    if (EVP_EncryptInit_ex(ctx, EVP_chacha20_poly1305(), nullptr, key.data(), iv.data()) != 1) {
        throw runtime_error("Failed to initialize encryption");
    }

    if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, 
                          reinterpret_cast<const unsigned char*>(message.data()), message.size()) != 1) {
        throw runtime_error("Failed to encrypt message");
    }
    ciphertext_len = len;

    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1) {
        throw runtime_error("Failed to finalize encryption");
    }
    ciphertext_len += len;

    ciphertext.resize(ciphertext_len);
    
    // Prepend IV to ciphertext
    ciphertext.insert(ciphertext.begin(), iv.begin(), iv.end());

    return base64Encode(ciphertext);
}

string MessageComposer::decryptMessage(const string& encryptedMessage) {
    vector<unsigned char> ciphertext;
    try {
        ciphertext = base64Decode(encryptedMessage);
    } catch (const exception& e) {
        throw runtime_error("Failed to decode Base64 message: " + string(e.what()));
    }

    if (ciphertext.size() <= 12) {  // 12 is the IV size
        throw runtime_error("Encrypted message is too short");
    }

    // Extract IV from ciphertext
    vector<unsigned char> message_iv(ciphertext.begin(), ciphertext.begin() + 12);
    ciphertext.erase(ciphertext.begin(), ciphertext.begin() + 12);

    vector<unsigned char> plaintext(ciphertext.size());
    int len, plaintext_len;

    if (EVP_DecryptInit_ex(ctx, EVP_chacha20_poly1305(), nullptr, key.data(), message_iv.data()) != 1) {
        throw runtime_error("Failed to initialize decryption");
    }

    if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size()) != 1) {
        throw runtime_error("Failed to decrypt message");
    }
    plaintext_len = len;

    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1) {
        throw runtime_error("Failed to finalize decryption");
    }
    plaintext_len += len;

    return string(plaintext.begin(), plaintext.begin() + plaintext_len);
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
    BIO_free_all(bio);

    if (decoded_size <= 0) {
        throw runtime_error("Failed to decode Base64 input");
    }

    result.resize(decoded_size);
    return result;
}