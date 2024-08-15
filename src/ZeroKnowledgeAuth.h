#ifndef ZERO_KNOWLEDGE_AUTH_H
#define ZERO_KNOWLEDGE_AUTH_H

#include <vector>
#include <cstdint>
#include <random>
#include <stdexcept>
#include <string>
#include <iostream>
#include <utility>

class ZeroKnowledgeAuth {
private:
    static constexpr int PRIME = 1000000007;  // A large prime number
    static constexpr int NUM_ROUNDS = 20;  // Number of challenge-response rounds
    static constexpr int KEY_SIZE = 4;  // Size of secret and public keys

    std::vector<int> secretKey;
    std::vector<int> publicKey;
    mutable std::mt19937 rng;

    int modularExponentiation(int base, int exponent, int modulus) const;

public:
    ZeroKnowledgeAuth();
    std::vector<int> getPublicKey() const { return publicKey; }
    std::pair<int, int> proveIdentity(const std::vector<int>& verifierPublicKey, const std::vector<bool>& challenge);
    bool verifyIdentity(const std::vector<int>& proverPublicKey, int commitment, int response, const std::vector<bool>& challenge);
    static std::vector<bool> generateChallenge();
    void printPublicKey() const;
    std::pair<int, int> generateCommitment() const;
    int generateResponse(int r, const std::vector<bool>& challenge) const;
    bool verify(int commitment, int response, const std::vector<int>& proverPublicKey, const std::vector<bool>& challenge) const;

};
#endif