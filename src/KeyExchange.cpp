#include "KeyExchange.h"
#include <stdexcept>

KeyExchange::KeyExchange() {
    rsa = RSA_generate_key(2048, RSA_F4, nullptr, nullptr);
    if (!rsa) {
        throw std::runtime_error("Failed to generate RSA key pair");
    }

    BIO *pubkey = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pubkey, rsa);
    char *pubkey_char;
    long pubkey_size = BIO_get_mem_data(pubkey, &pubkey_char);
    public_key = std::string(pubkey_char, pubkey_size);
    BIO_free(pubkey);

    BIO *privkey = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPrivateKey(privkey, rsa, nullptr, nullptr, 0, nullptr, nullptr);
    char *privkey_char;
    long privkey_size = BIO_get_mem_data(privkey, &privkey_char);
    private_key = std::string(privkey_char, privkey_size);
    BIO_free(privkey);
}

KeyExchange::~KeyExchange() {
    RSA_free(rsa);
}

std::string KeyExchange::getPublicKey() const {
    return public_key;
}

std::vector<unsigned char> KeyExchange::encryptKey(const std::vector<unsigned char>& key, const std::string& recipient_public_key) {
    BIO *keybio = BIO_new_mem_buf(recipient_public_key.c_str(), -1);
    RSA *rsa_public = PEM_read_bio_RSAPublicKey(keybio, nullptr, nullptr, nullptr);
    BIO_free(keybio);

    std::vector<unsigned char> encrypted(RSA_size(rsa_public));
    int encrypted_length = RSA_public_encrypt(key.size(), key.data(), encrypted.data(), rsa_public, RSA_PKCS1_OAEP_PADDING);
    
    if (encrypted_length == -1) {
        RSA_free(rsa_public);
        throw std::runtime_error("Failed to encrypt key");
    }

    encrypted.resize(encrypted_length);
    RSA_free(rsa_public);
    return encrypted;
}

std::vector<unsigned char> KeyExchange::decryptKey(const std::vector<unsigned char>& encrypted_key) {
    std::vector<unsigned char> decrypted(RSA_size(rsa));
    int decrypted_length = RSA_private_decrypt(encrypted_key.size(), encrypted_key.data(), decrypted.data(), rsa, RSA_PKCS1_OAEP_PADDING);
    
    if (decrypted_length == -1) {
        throw std::runtime_error("Failed to decrypt key");
    }

    decrypted.resize(decrypted_length);
    return decrypted;
}
