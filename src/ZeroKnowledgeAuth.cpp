#include "ZeroKnowledgeAuth.h"
#include <algorithm>
#include <iostream>
#include <cassert>

ZeroKnowledgeAuth::ZeroKnowledgeAuth() : rng(std::random_device{}()) {
    std::uniform_int_distribution<int> dist(1, PRIME-1);
    for (int i = 0; i < KEY_SIZE; ++i) {
        secretKey.push_back(dist(rng));
        publicKey.push_back(modularExponentiation(secretKey[i], 2, PRIME));
    }
    std::cout << "Generated secret key: ";
    for (int key : secretKey) std::cout << key << " ";
    std::cout << "\nGenerated public key: ";
    for (int key : publicKey) std::cout << key << " ";
    std::cout << std::endl;
}

int ZeroKnowledgeAuth::modularExponentiation(int base, int exponent, int modulus) const {
    long long result = 1;
    long long b = base % modulus;
    while (exponent > 0) {
        if (exponent & 1)
            result = (result * b) % modulus;
        exponent >>= 1;
        b = (b * b) % modulus;
    }
    return static_cast<int>(result);
}




std::vector<bool> ZeroKnowledgeAuth::generateChallenge() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::vector<bool> challenge(KEY_SIZE);
    std::uniform_int_distribution<int> dist(0, 1);
    for (int i = 0; i < KEY_SIZE; ++i) {
        challenge[i] = dist(gen) == 1;
    }
    return challenge;
}




std::pair<int, int> ZeroKnowledgeAuth::generateCommitment() const {
    std::uniform_int_distribution<int> dist(1, PRIME-1);
    int r = dist(rng);
    int x = modularExponentiation(r, 2, PRIME);
    return {r, x};
}

int ZeroKnowledgeAuth::generateResponse(int r, const std::vector<bool>& challenge) const {
    long long y = r;
    for (int i = 0; i < KEY_SIZE; ++i) {
        if (challenge[i]) {
            y = (y * secretKey[i]) % PRIME;
        }
    }
    return static_cast<int>(y);
}

bool ZeroKnowledgeAuth::verify(int commitment, int response, const std::vector<int>& proverPublicKey, const std::vector<bool>& challenge) const {
    int left = modularExponentiation(response, 2, PRIME);
    long long right = commitment;
    for (int i = 0; i < KEY_SIZE; ++i) {
        if (challenge[i]) {
            right = (right * proverPublicKey[i]) % PRIME;
        }
    }
    return left == right;
}




std::pair<int, int> ZeroKnowledgeAuth::proveIdentity(const std::vector<int>& verifierPublicKey, const std::vector<bool>& challenge) {
    std::cout << "Prover's public key: ";
    printPublicKey();
    std::cout << "Verifier's public key: ";
    for (int key : verifierPublicKey) std::cout << key << " ";
    std::cout << std::endl;

    std::uniform_int_distribution<int> dist(1, PRIME-1);
    int r = dist(rng);
    int x = modularExponentiation(r, 2, PRIME);
    std::cout << "Commitment: " << x << std::endl;

    std::cout << "Challenge: ";
    for (bool b : challenge) std::cout << b << " ";
    std::cout << std::endl;

    long long y = r;
    for (int i = 0; i < KEY_SIZE; ++i) {
        if (challenge[i]) {
            y = (y * secretKey[i]) % PRIME;
        }
    }
    int response = static_cast<int>(y);
    std::cout << "Response: " << response << std::endl;

    return {x, response};
}



bool ZeroKnowledgeAuth::verifyIdentity(const std::vector<int>& proverPublicKey, int commitment, int response, const std::vector<bool>& challenge) {
    std::cout << "Verifier's public key: ";
    printPublicKey();
    std::cout << "Prover's public key: ";
    for (int key : proverPublicKey) std::cout << key << " ";
    std::cout << std::endl;

    std::cout << "Received commitment: " << commitment << std::endl;

    std::cout << "Challenge: ";
    for (bool b : challenge) std::cout << b << " ";
    std::cout << std::endl;

    std::cout << "Received response: " << response << std::endl;

    int left = modularExponentiation(response, 2, PRIME);
    long long right = commitment;
    for (int i = 0; i < KEY_SIZE; ++i) {
        if (challenge[i]) {
            right = (right * proverPublicKey[i]) % PRIME;
        }
    }
    
    bool verified = (left == right);
    std::cout << "Verification: " << (verified ? "Passed" : "Failed") << std::endl;
    return verified;
}


void ZeroKnowledgeAuth::printPublicKey() const {
    std::cout << "Public key: ";
    for (int key : publicKey) {
        std::cout << key << " ";
    }
    std::cout << std::endl;
}


