#include "HammingCode.h"
#include <iostream>
using namespace std;

vector<uint8_t> HammingCode::encode(const vector<uint8_t>& data) {
    vector<uint8_t> encoded;
    for (uint8_t byte : data) {
        for (int i = 0; i < 2; i++) {
            uint8_t nibble = (byte >> (4 * i)) & 0x0F;
            uint8_t encoded_nibble = nibble | (calculateParity(nibble) << 4);
            encoded.push_back(encoded_nibble);
        }
    }
    cout << "HammingCode::encode - Input size: " << data.size() << ", Output size: " << encoded.size() << endl;
    return encoded;
}


vector<uint8_t> HammingCode::decode(const vector<uint8_t>& encoded) {
    vector<uint8_t> decoded;
    for (size_t i = 0; i < encoded.size(); i += 2) {
        uint8_t byte = 0;
        for (int j = 0; j < 2; j++) {
            uint8_t encoded_nibble = encoded[i + j];
            uint8_t data = encoded_nibble & 0x0F;
            uint8_t parity = encoded_nibble >> 4;
            uint8_t syndrome = parity ^ calculateParity(data);
            uint8_t corrected = correctError(data, syndrome);
            byte |= (corrected << (4 * j));
        }
        decoded.push_back(byte);
    }
    return decoded;
}

uint8_t HammingCode::correctError(uint8_t data, uint8_t syndrome) {
    if (syndrome != 0) {
        uint8_t errorBit = syndrome - 1;
        if (errorBit < 4) {  // Only correct errors in the data bits
            data ^= (1 << errorBit);
        }
    }
    return data;
}



uint8_t HammingCode::calculateParity(uint8_t data) {
    uint8_t p1 = (data ^ (data >> 1) ^ (data >> 2) ^ (data >> 3)) & 0x01;
    uint8_t p2 = (data ^ (data >> 1) ^ (data >> 2)) & 0x01;
    uint8_t p3 = (data ^ (data >> 1) ^ (data >> 3)) & 0x01;
    return (p3 << 2) | (p2 << 1) | p1;
}

